#include "controller/PlayingMediaController.h"
#include "view/PlayingView.h"
#include "controller/PlayingMediaController.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <iostream>
#include <string>
#include <sstream>
#include "common/TerminalUtils.h"

bool isSDLInitialized = false;
Mix_Music* currentMusic = nullptr;
std::atomic<bool> isVideoPlaying{false};
void initializeSDL() {
    if (SDL_Init(SDL_INIT_AUDIO | SDL_INIT_VIDEO) < 0) {
        std::cerr << "Failed to initialize SDL: " << SDL_GetError() << "\n";
        return;
    }
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        std::cerr << "Failed to initialize SDL_mixer: " << Mix_GetError() << "\n";
        return;
    }
    isSDLInitialized = true;
}

void cleanupSDL() {
    if (currentMusic) {
        Mix_FreeMusic(currentMusic);
        currentMusic = nullptr;
    }
    Mix_CloseAudio();
    SDL_Quit();
}

void PlayingMediaController::handleAction(int choice) {
    switch (choice) {
        case ACTION_PLAY_PAUSE: {
            if (Mix_PausedMusic()) {
                Mix_ResumeMusic();
                isPlaying = true;
                cout << "Resuming music...\n";
            } else {
                Mix_PauseMusic();
                isPlaying = false;
                cout << "Pausing music...\n";
            }
            break;
        }
        case ACTION_SKIP_NEXT:
            skipToNext();
            break;
        case ACTION_SKIP_PREVIOUS:
            skipToPrevious();
            break;
        case ACTION_ADJUST_VOLUME: {
            int newVolume;
            std::cout << "Enter new volume (0-100): ";
            std::cin >> newVolume;
            adjustVolume(newVolume);
            break;
        }
        case ACTION_STOP: stop(); break;
        case ACTION_EXIT_PLAYING_MENU: {
            isRunning = false;
            clearTerminal();
            ManagerController::getInstance().getManagerView()->setView("Default");
        }
        default:
            break;
    }
}

void PlayingMediaController::adjustVolume(int level) {
    if (level < 0 || level > 100) {
        std::cerr << "Volume level must be between 0 and 100.\n";
        return;
    }

    volume = level;
    Mix_VolumeMusic((MIX_MAX_VOLUME * volume) / 100);
}

void PlayingMediaController::stop() {
    if (!isPlaying && !isVideoPlaying) return;

    isRunning = false;

    if (updateThread.joinable() && std::this_thread::get_id() != updateThread.get_id()) {
        updateThread.join();
    }

    if (Mix_PlayingMusic()) {
        Mix_HaltMusic();
    }

    if (currentMusic) {
        Mix_FreeMusic(currentMusic);
        currentMusic = nullptr;
    }

    isPlaying = false;
    isVideoPlaying = false;
}

void PlayingMediaController::skipToNext() {
    stop();

    if (currentPlaylist && !currentPlaylist->empty()) {
        currentPlaylistIndex = (currentPlaylistIndex + 1) % currentPlaylist->size();
        currentMediaFile = &(*currentPlaylist)[currentPlaylistIndex];
    } else {
        auto& mediaFiles = ManagerController::getInstance().getManagerModel()->getMediaLibrary().getMediaFiles();
        if (mediaFiles.empty()) {
            std::cerr << "No media files available to skip.\n";
            return;
        }

        if (!currentMediaFile) {
            currentMediaFile = &mediaFiles.front();
        } else {
            auto it = std::find_if(mediaFiles.begin(), mediaFiles.end(),
                                   [this](const MediaFile& file) { return file.getPath() == currentMediaFile->getPath(); });
            if (it != mediaFiles.end()) {
                ++it;
                currentMediaFile = (it != mediaFiles.end()) ? &(*it) : &mediaFiles.front();
            } else {
                currentMediaFile = &mediaFiles.front();
            }
        }
    }

    playMediaFile(currentMediaFile);
}


void PlayingMediaController::skipToPrevious() {
    stop();

    if (currentPlaylist && !currentPlaylist->empty()) {
        currentPlaylistIndex = (currentPlaylistIndex == 0) ? currentPlaylist->size() - 1 : currentPlaylistIndex - 1;
        currentMediaFile = &(*currentPlaylist)[currentPlaylistIndex];
    } else {
        auto& mediaFiles = ManagerController::getInstance().getManagerModel()->getMediaLibrary().getMediaFiles();
        if (mediaFiles.empty()) {
            std::cerr << "No media files available to skip.\n";
            return;
        }

        if (!currentMediaFile) {
            currentMediaFile = &mediaFiles.back();
        } else {
            auto it = std::find_if(mediaFiles.begin(), mediaFiles.end(),
                                   [this](const MediaFile& file) { return file.getPath() == currentMediaFile->getPath(); });
            if (it != mediaFiles.end() && it != mediaFiles.begin()) {
                --it;
                currentMediaFile = &(*it);
            } else {
                currentMediaFile = &mediaFiles.back();
            }
        }
    }

    playMediaFile(currentMediaFile);
}

void PlayingMediaController::playMediaFile(MediaFile* mediaFile) {
    if (!mediaFile) {
        std::cerr << "No media file provided.\n";
        return;
    }

    std::string path = mediaFile->getPath();
    std::string extension = path.substr(path.find_last_of('.') + 1);

    if (extension == "mp3" || extension == "wav" || extension == "ogg") {
        stop();
        isRunning = true;

        if (!isSDLInitialized) {
            initializeSDL();
            if (!isSDLInitialized) {
                std::cerr << "Failed to initialize SDL.\n";
                return;
            }
        }

        currentMusic = Mix_LoadMUS(path.c_str());
        if (!currentMusic) {
            std::cerr << "Failed to load music file: " << path << " (" << Mix_GetError() << ")\n";
            return;
        }

        if (Mix_PlayMusic(currentMusic, 1) == -1) {
            std::cerr << "Failed to play music: " << Mix_GetError() << "\n";
            return;
        }

        Mix_HookMusicFinished([]() {
            PlayingMediaController* controller = dynamic_cast<PlayingMediaController*>(
                ManagerController::getInstance().getController("PlayingMedia"));
            if (controller) {
                controller->skipToNext();
            }
        });

        currentMediaFile = mediaFile;
        isPlaying = true;

        auto fileRef = TagLib::FileRef(path.c_str());
        totalTime = fileRef.audioProperties() ? fileRef.audioProperties()->length() : 0;
        currentTime = 0;

        startDisplayLoop();

    } else if (extension == "mp4" || extension == "avi" || extension == "mkv") {
        isVideoPlaying = true;
        std::thread videoThread([this, path]() {
            playVideo(path);
            isVideoPlaying = false;
            SDL_Event event;
            event.type = SDL_USEREVENT;
            event.user.code = 1;
            SDL_PushEvent(&event);
        });
        videoThread.detach();

    } else {
        std::cerr << "Unsupported file format: " << extension << "\n";
    }
}



void PlayingMediaController::startDisplayLoop() {
    if (updateThread.joinable()) {
        std::cout << "Display loop is already running. Ensuring updates...\n";
        isRunning = true;
        return;
    }

    PlayingView* playingView = dynamic_cast<PlayingView*>(
        ManagerController::getInstance().getManagerView()->getView());
    if (!playingView) {
        std::cerr << "PlayingView not available.\n";
        return;
    }

    updateThread = std::thread([this, playingView]() {
        while (isRunning) {
            {
                std::lock_guard<std::mutex> lock(mediaMutex);
                playingView->clearView();
                playingView->displayPlayingView(
                    currentMediaFile ? currentMediaFile->getName() : "No Media",
                    totalTime.load(),
                    volume,
                    currentTime.load()
                );
            }

            std::this_thread::sleep_for(std::chrono::seconds(1));

            if (isPlaying) {
                ++currentTime;
                if (currentTime >= totalTime) {
                    std::cout << "\nPlayback finished. Skipping to next track...\n";
                    isRunning = false;
                    skipToNext();
                }
            }
        }
    });
}

void PlayingMediaController::playPlaylist(std::vector<MediaFile>& playlist) {
    currentPlaylist = &playlist;
    currentPlaylistIndex = 0;
    if (!playlist.empty()) {
        currentMediaFile = &playlist[currentPlaylistIndex];
        playMediaFile(currentMediaFile);
    } else {
        std::cerr << "Playlist is empty.\n";
    }
}

MediaFile* PlayingMediaController::getCurrentMediaFile() const {
    return currentMediaFile;
}

bool PlayingMediaController::isCurrentlyPlaying() const {
    return isPlaying;
}

size_t PlayingMediaController::getCurrentPlaylistIndex() const {
    return currentPlaylistIndex;
}

std::vector<MediaFile>* PlayingMediaController::getCurrentPlaylist() const {
    return currentPlaylist;
}
void PlayingMediaController::setCurrentMediaFile(MediaFile* mediaFile) {
    std::lock_guard<std::mutex> lock(mediaMutex);
    currentMediaFile = mediaFile;
}

void PlayingMediaController::setIsPlaying(bool playing) {
    std::lock_guard<std::mutex> lock(mediaMutex);
    isPlaying = playing;
}

void PlayingMediaController::setCurrentPlaylist(std::vector<MediaFile>* playlist) {
    std::lock_guard<std::mutex> lock(mediaMutex);
    currentPlaylist = playlist;
}

void PlayingMediaController::setCurrentPlaylistIndex(size_t index) {
    std::lock_guard<std::mutex> lock(mediaMutex);
    currentPlaylistIndex = index;
}

int PlayingMediaController::getTotalTime() const {
    return totalTime.load();
}

int PlayingMediaController::getCurrentTime() const {
    return currentTime.load();
}

int PlayingMediaController::getVolume() const {
    return volume;
}

void PlayingMediaController::refreshPlayingView() {
    PlayingView* playingView = dynamic_cast<PlayingView*>(
        ManagerController::getInstance().getManagerView()->getView());
    if (playingView && currentMediaFile) {
        playingView->clearView();
        playingView->displayPlayingView(
            currentMediaFile->getName(),
            totalTime.load(),
            volume,
            currentTime.load()
        );
    }
}

void PlayingMediaController::playVideo(const std::string& videoPath) {
    AVFormatContext* formatContext = avformat_alloc_context();
    if (avformat_open_input(&formatContext, videoPath.c_str(), nullptr, nullptr) != 0) {
        std::cerr << "Failed to open video file: " << videoPath << "\n";
        return;
    }

    if (avformat_find_stream_info(formatContext, nullptr) < 0) {
        std::cerr << "Failed to retrieve stream info.\n";
        avformat_close_input(&formatContext);
        return;
    }

    int videoStreamIndex = -1;
    for (unsigned int i = 0; i < formatContext->nb_streams; i++) {
        if (formatContext->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_VIDEO) {
            videoStreamIndex = i;
            break;
        }
    }

    if (videoStreamIndex == -1) {
        std::cerr << "No video stream found.\n";
        avformat_close_input(&formatContext);
        return;
    }

    AVCodecParameters* codecParams = formatContext->streams[videoStreamIndex]->codecpar;
    AVStream* videoStream = formatContext->streams[videoStreamIndex];
    AVCodec* codec = avcodec_find_decoder(codecParams->codec_id);
    if (!codec) {
        std::cerr << "Unsupported codec.\n";
        avformat_close_input(&formatContext);
        return;
    }

    AVCodecContext* codecContext = avcodec_alloc_context3(codec);
    if (avcodec_parameters_to_context(codecContext, codecParams) < 0) {
        std::cerr << "Failed to copy codec parameters.\n";
        avcodec_free_context(&codecContext);
        avformat_close_input(&formatContext);
        return;
    }

    if (avcodec_open2(codecContext, codec, nullptr) < 0) {
        std::cerr << "Failed to open codec.\n";
        avcodec_free_context(&codecContext);
        avformat_close_input(&formatContext);
        return;
    }

    if (SDL_Init(SDL_INIT_VIDEO) != 0 || TTF_Init() != 0) {
        std::cerr << "Failed to initialize SDL or SDL_ttf: " << SDL_GetError() << "\n";
        avcodec_free_context(&codecContext);
        avformat_close_input(&formatContext);
        return;
    }

    SDL_Window* window = SDL_CreateWindow("Video Player", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_SHOWN);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 0);
    SDL_Texture* texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_YV12, SDL_TEXTUREACCESS_STREAMING, codecParams->width, codecParams->height);

    TTF_Font* font = TTF_OpenFont("/usr/share/fonts/truetype/dejavu/DejaVuSerif.ttf", 24);
    if (!font) {
        std::cerr << "Failed to load font: " << TTF_GetError() << "\n";
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        avcodec_free_context(&codecContext);
        avformat_close_input(&formatContext);
        return;
    }

    SDL_Color white = {255, 255, 255, 255};
    std::ostringstream infoStream;
    infoStream << "Resolution: " << codecParams->width << "x" << codecParams->height << "\n"
               << "Codec: " << avcodec_get_name(codecParams->codec_id) << "\n"
               << "Frame Rate: " << av_q2d(videoStream->avg_frame_rate) << " FPS\n"
               << "Duration: " << (formatContext->duration / AV_TIME_BASE) << " seconds";

    std::string videoInfo = infoStream.str();
    SDL_Surface* textSurface = TTF_RenderText_Blended_Wrapped(font, videoInfo.c_str(), white, 800);
    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    SDL_Rect textRect = {10, 10, textSurface->w, textSurface->h};
    SDL_FreeSurface(textSurface);

    AVFrame* frame = av_frame_alloc();
    AVFrame* frameYUV = av_frame_alloc();
    SwsContext* swsContext = sws_getContext(
        codecParams->width, codecParams->height, codecContext->pix_fmt,
        codecParams->width, codecParams->height, AV_PIX_FMT_YUV420P,
        SWS_BILINEAR, nullptr, nullptr, nullptr);

    int bufferSize = av_image_get_buffer_size(AV_PIX_FMT_YUV420P, codecParams->width, codecParams->height, 1);
    uint8_t* buffer = (uint8_t*)av_malloc(bufferSize);
    av_image_fill_arrays(frameYUV->data, frameYUV->linesize, buffer, AV_PIX_FMT_YUV420P, codecParams->width, codecParams->height, 1);
    AVPacket* packet = av_packet_alloc();
    SDL_Event event;
    isVideoPlaying = true;

    while (isVideoPlaying) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                isVideoPlaying = false;
            } else if (event.type == SDL_KEYDOWN) {
                if (event.key.keysym.sym == SDLK_ESCAPE) {
                    isVideoPlaying = false;
                }
            } else if (event.type == SDL_USEREVENT && event.user.code == 1) {
                auto* controller = dynamic_cast<PlayingMediaController*>(
                    ManagerController::getInstance().getController("PlayingMedia"));
                if (controller) {
                    controller->skipToNext();
                }
            }
        }

        int ret = av_read_frame(formatContext, packet);
        if (ret < 0) {
            if (ret == AVERROR_EOF) {
                std::cout << "End of video file reached. Skipping to next media...\n";
                isVideoPlaying = false;

                SDL_Event event;
                event.type = SDL_USEREVENT;
                event.user.code = 1;
                SDL_PushEvent(&event);
            } else {
                std::cerr << "Error reading frame: " << ret << "\n";
            }
            break;
        }

        if (packet->stream_index == videoStreamIndex) {
            if (avcodec_send_packet(codecContext, packet) >= 0) {
                while (avcodec_receive_frame(codecContext, frame) >= 0) {
                    sws_scale(swsContext, frame->data, frame->linesize, 0, codecParams->height,
                            frameYUV->data, frameYUV->linesize);
                    SDL_UpdateYUVTexture(texture, nullptr, frameYUV->data[0], frameYUV->linesize[0],
                                        frameYUV->data[1], frameYUV->linesize[1], frameYUV->data[2], frameYUV->linesize[2]);
                    SDL_RenderClear(renderer);
                    SDL_RenderCopy(renderer, texture, nullptr, nullptr);
                    SDL_RenderCopy(renderer, textTexture, nullptr, &textRect);
                    SDL_RenderPresent(renderer);
                    SDL_Delay(40);
                }
            }
        }
        av_packet_unref(packet);
    }

    SDL_DestroyTexture(textTexture);
    TTF_CloseFont(font);
    av_packet_free(&packet);
    av_free(buffer);
    av_frame_free(&frameYUV);
    av_frame_free(&frame);
    sws_freeContext(swsContext);
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();
    avcodec_free_context(&codecContext);
    avformat_close_input(&formatContext);
}
