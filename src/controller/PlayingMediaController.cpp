#include "controller/PlayingMediaController.h"
#include "view/PlayingView.h"
#include "controller/PlayingMediaController.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <iostream>
#include <string>
#include <sstream>
#include "common/TerminalUtils.h"

/* Global variables for SDL initialization and state tracking */
bool isSDLInitialized = false;
Mix_Music* currentMusic = nullptr;
atomic<bool> isVideoPlaying{false};

/* Function to initialize SDL and SDL_mixer */
void initializeSDL() {
    if (SDL_Init(SDL_INIT_AUDIO | SDL_INIT_VIDEO) < 0) {
        cerr << "Failed to initialize SDL: " << SDL_GetError() << "\n";
        return;
    }
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        cerr << "Failed to initialize SDL_mixer: " << Mix_GetError() << "\n";
        return;
    }
    isSDLInitialized = true;
}

/* Function to clean up SDL resources */
void cleanupSDL() {
    if (currentMusic) {
        Mix_FreeMusic(currentMusic);
        currentMusic = nullptr;
    }
    Mix_CloseAudio();
    SDL_Quit();
}

/* Function to play a video file */
void PlayingMediaController::handleAction(int choice) {
    switch (choice) {
        case ACTION_PLAY_PAUSE: {
            /* Pause or resume the music */
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
            /* Skip to the next track */
            skipToNext();
            break;
        case ACTION_SKIP_PREVIOUS:
            /* Skip to the previous track */
            skipToPrevious();
            break;
        case ACTION_ADJUST_VOLUME: {
            /* Adjust the volume level */
            int newVolume;
            cout << "Enter new volume (0-100): ";
            cin >> newVolume;
            adjustVolume(newVolume);
            break;
        }
        case ACTION_STOP:
            /* Stop the music */
            stop();
            break;
        case ACTION_EXIT_PLAYING_MENU: {
            /* Stop the music and return to the home screen */
            isRunning = false;
            clearTerminal();
            ManagerController::getInstance().getManagerView()->setView("Default");
        }
        default:
            break;
    }
}

/* Function to adjust the volume level */
void PlayingMediaController::adjustVolume(int level) {
    if (level < 0 || level > 100) {
        cerr << "Volume level must be between 0 and 100.\n";
        return;
    }

    volume = level;
    Mix_VolumeMusic((MIX_MAX_VOLUME * volume) / 100);
}

/* Function to stop the music */
void PlayingMediaController::stop() {
    if (!isPlaying) return;

    isRunning = false;

    /* Wait for the update thread to finish */
    if (updateThread.joinable() && this_thread::get_id() != updateThread.get_id()) {
        updateThread.join();
    }

    /* Stop the music */
    if (Mix_PlayingMusic()) {
        Mix_HaltMusic();
    }

    /* Free the music resource */
    if (currentMusic) {
        Mix_FreeMusic(currentMusic);
        currentMusic = nullptr;
    }

    /* Reset the current media file */
    PlayingView* playingView = dynamic_cast<PlayingView*>(
        ManagerController::getInstance().getManagerView()->getView());
    if (playingView) {
        playingView->clearView();
        playingView->displayPlayingView("No Media", 0, volume, 0);
    }

    isPlaying = false;
}

/* Function to skip to the next track */
void PlayingMediaController::skipToNext() {
    stop();

    /* Skip to the next track in the playlist */
    if (currentPlaylist && !currentPlaylist->empty()) {
        currentPlaylistIndex = (currentPlaylistIndex + 1) % currentPlaylist->size();
        currentMediaFile = &(*currentPlaylist)[currentPlaylistIndex];
    } else {
        /* Skip to the next track in the media library */
        auto& mediaFiles = ManagerController::getInstance().getManagerModel()->getMediaLibrary().getMediaFiles();
        if (mediaFiles.empty()) {
            cerr << "No media files available to skip.\n";
            return;
        }

        if (!currentMediaFile) {
            currentMediaFile = &mediaFiles.front();
        } else {
            auto it = find_if(mediaFiles.begin(), mediaFiles.end(),
                                   [this](const MediaFile& file) { return file.getPath() == currentMediaFile->getPath(); });
            if (it != mediaFiles.end()) {
                ++it;
                currentMediaFile = (it != mediaFiles.end()) ? &(*it) : &mediaFiles.front();
            } else {
                currentMediaFile = &mediaFiles.front();
            }
        }
    }

    /* Play the next track */
    playMediaFile(currentMediaFile);
}

/* Function to skip to the previous track */
void PlayingMediaController::skipToPrevious() {
    /* Stop the current music */
    stop();

    /* Skip to the previous track in the playlist */
    if (currentPlaylist && !currentPlaylist->empty()) {
        currentPlaylistIndex = (currentPlaylistIndex == 0) ? currentPlaylist->size() - 1 : currentPlaylistIndex - 1;
        currentMediaFile = &(*currentPlaylist)[currentPlaylistIndex];
    } else {
        /* Skip to the previous track in the media library */
        auto& mediaFiles = ManagerController::getInstance().getManagerModel()->getMediaLibrary().getMediaFiles();
        if (mediaFiles.empty()) {
            cerr << "No media files available to skip.\n";
            return;
        }

        if (!currentMediaFile) {
            currentMediaFile = &mediaFiles.back();
        } else {
            auto it = find_if(mediaFiles.begin(), mediaFiles.end(),
                                   [this](const MediaFile& file) { return file.getPath() == currentMediaFile->getPath(); });
            if (it != mediaFiles.end() && it != mediaFiles.begin()) {
                --it;
                currentMediaFile = &(*it);
            } else {
                currentMediaFile = &mediaFiles.back();
            }
        }
    }

    /* Play the previous track */
    playMediaFile(currentMediaFile);
}

/* Function to play a media file */
void PlayingMediaController::playMediaFile(MediaFile* mediaFile) {
    if (!mediaFile) {
        cerr << "No media file provided.\n";
        return;
    }

    /* Get the file path and its extension */
    string path = mediaFile->getPath();
    string extension = path.substr(path.find_last_of('.') + 1);

    /* Check if the media file is a supported audio format */
    if (extension == "mp3" || extension == "wav" || extension == "ogg") {
        stop();
        isRunning = true;

        if (!isSDLInitialized) {
            initializeSDL();
            if (!isSDLInitialized) {
                cerr << "Failed to initialize SDL.\n";
                return;
            }
        }

        /* Load the music file */
        currentMusic = Mix_LoadMUS(path.c_str());
        if (!currentMusic) {
            cerr << "Failed to load music file: " << path << " (" << Mix_GetError() << ")\n";
            return;
        }

        /* Start playing the music */
        if (Mix_PlayMusic(currentMusic, 1) == -1) {
            cerr << "Failed to play music: " << Mix_GetError() << "\n";
            return;
        }

        /* Set a callback for when the music finishes */
        Mix_HookMusicFinished([]() {
            PlayingMediaController* controller = dynamic_cast<PlayingMediaController*>(
                ManagerController::getInstance().getController("PlayingMedia"));
            if (controller) {
                controller->skipToNext();
            }
        });

        /* Update the current media file and state */
        currentMediaFile = mediaFile;
        isPlaying = true;

        /* Get the total time of the music file */
        auto fileRef = TagLib::FileRef(path.c_str());
        totalTime = fileRef.audioProperties() ? fileRef.audioProperties()->length() : 0;
        currentTime = 0;

        /* Start the display loop */
        startDisplayLoop();

    } else if (extension == "mp4" || extension == "avi" || extension == "mkv") {
        /* Play the video file */
        isVideoPlaying = true;
        thread videoThread([this, path]() {
            playVideo(path);
            isVideoPlaying = false;

            /* Trigger an SDL user event to indicate video completion */
            SDL_Event event;
            event.type = SDL_USEREVENT;
            event.user.code = 1;
            SDL_PushEvent(&event);
        });
        videoThread.detach();

    } else {
        cerr << "Unsupported file format: " << extension << "\n";
    }
}

/* Start a loop to continuously update the playing view */
void PlayingMediaController::startDisplayLoop() {
    if (updateThread.joinable()) {
        /* If a display loop is already running, ensure it updates correctly */
        cout << "Display loop is already running. Ensuring updates...\n";
        isRunning = true;
        return;
    }

    /* Retrieve the PlayingView instance */
    PlayingView* playingView = dynamic_cast<PlayingView*>(
        ManagerController::getInstance().getManagerView()->getView());
    if (!playingView) {
        cerr << "PlayingView not available.\n";
        return;
    }

    /* Start a new thread for updating the playing view */
    updateThread = thread([this, playingView]() {
        while (isRunning) {
            {
                lock_guard<mutex> lock(mediaMutex);

                /* Update the playing view with the current media details */
                playingView->clearView();
                playingView->displayPlayingView(
                    currentMediaFile ? currentMediaFile->getName() : "No Media",
                    totalTime.load(),
                    volume,
                    currentTime.load()
                );
            }

            /* Wait for 1 second before updating the view again */
            this_thread::sleep_for(chrono::seconds(1));

            if (isPlaying) {
                ++currentTime;
                if (currentTime >= totalTime) {
                    /* If playback is finished, skip to the next track */
                    cout << "\nPlayback finished. Skipping to next track...\n";
                    isRunning = false;
                    skipToNext();
                }
            }
        }
    });
}

/* Play a playlist of media files */
void PlayingMediaController::playPlaylist(vector<MediaFile>& playlist) {
    currentPlaylist = &playlist;
    currentPlaylistIndex = 0;

    if (!playlist.empty()) {
        /* Play the first media file in the playlist */
        currentMediaFile = &playlist[currentPlaylistIndex];
        playMediaFile(currentMediaFile);
    } else {
        cerr << "Playlist is empty.\n";
    }
}

/* Getter for the currently playing media file */
MediaFile* PlayingMediaController::getCurrentMediaFile() const {
    return currentMediaFile;
}

/* Check if media is currently playing */
bool PlayingMediaController::isCurrentlyPlaying() const {
    return isPlaying;
}

/* Get the index of the currently playing item in the playlist */
size_t PlayingMediaController::getCurrentPlaylistIndex() const {
    return currentPlaylistIndex;
}

/* Get the current playlist */
vector<MediaFile>* PlayingMediaController::getCurrentPlaylist() const {
    return currentPlaylist;
}

/* Set the currently playing media file */
void PlayingMediaController::setCurrentMediaFile(MediaFile* mediaFile) {
    lock_guard<mutex> lock(mediaMutex);
    currentMediaFile = mediaFile;
}

/* Set the playing state */
void PlayingMediaController::setIsPlaying(bool playing) {
    lock_guard<mutex> lock(mediaMutex);
    isPlaying = playing;
}

/* Set the current playlist */
void PlayingMediaController::setCurrentPlaylist(vector<MediaFile>* playlist) {
    lock_guard<mutex> lock(mediaMutex);
    currentPlaylist = playlist;
}

/* Set the current playlist index */
void PlayingMediaController::setCurrentPlaylistIndex(size_t index) {
    lock_guard<mutex> lock(mediaMutex);
    currentPlaylistIndex = index;
}

/* Get the total playback time of the current media */
int PlayingMediaController::getTotalTime() const {
    return totalTime.load();
}

/* Get the current playback time of the media */
int PlayingMediaController::getCurrentTime() const {
    return currentTime.load();
}

/* Get the current volume level */
int PlayingMediaController::getVolume() const {
    return volume;
}

/* Refresh the playing view with updated details */
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

/* Play a video file using FFmpeg and SDL */
void PlayingMediaController::playVideo(const string& videoPath) {
    /* Allocate the format context for the video file */
    AVFormatContext* formatContext = avformat_alloc_context();
    if (avformat_open_input(&formatContext, videoPath.c_str(), nullptr, nullptr) != 0) {
        cerr << "Failed to open video file: " << videoPath << "\n";
        return;
    }

    /* Retrieve stream information from the video file */
    if (avformat_find_stream_info(formatContext, nullptr) < 0) {
        cerr << "Failed to retrieve stream info.\n";
        avformat_close_input(&formatContext);
        return;
    }

    /* Identify the video stream in the file */
    int videoStreamIndex = -1;
    for (unsigned int i = 0; i < formatContext->nb_streams; i++) {
        if (formatContext->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_VIDEO) {
            videoStreamIndex = i;
            break;
        }
    }

    /* Check if a video stream was found */
    if (videoStreamIndex == -1) {
        cerr << "No video stream found.\n";
        avformat_close_input(&formatContext);
        return;
    }

    /* Retrieve the codec parameters for the video stream */
    AVCodecParameters* codecParams = formatContext->streams[videoStreamIndex]->codecpar;
    AVStream* videoStream = formatContext->streams[videoStreamIndex];
    const AVCodec* codec = avcodec_find_decoder(codecParams->codec_id);
    if (!codec) {
        cerr << "Unsupported codec.\n";
        avformat_close_input(&formatContext);
        return;
    }

    /* Allocate and configure the codec context */
    AVCodecContext* codecContext = avcodec_alloc_context3(codec);
    if (avcodec_parameters_to_context(codecContext, codecParams) < 0) {
        cerr << "Failed to copy codec parameters.\n";
        avcodec_free_context(&codecContext);
        avformat_close_input(&formatContext);
        return;
    }

    /* Open the codec for decoding */
    if (avcodec_open2(codecContext, codec, nullptr) < 0) {
        cerr << "Failed to open codec.\n";
        avcodec_free_context(&codecContext);
        avformat_close_input(&formatContext);
        return;
    }

    /* Initialize SDL for video rendering */
    if (SDL_Init(SDL_INIT_VIDEO) != 0 || TTF_Init() != 0) {
        cerr << "Failed to initialize SDL or SDL_ttf: " << SDL_GetError() << "\n";
        avcodec_free_context(&codecContext);
        avformat_close_input(&formatContext);
        return;
    }

    /* Create a window and renderer for displaying the video */
    SDL_Window* window = SDL_CreateWindow("Video Player", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_SHOWN);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 0);
    SDL_Texture* texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_YV12, SDL_TEXTUREACCESS_STREAMING, codecParams->width, codecParams->height);

    /* Load a font for displaying video information */
    TTF_Font* font = TTF_OpenFont("/usr/share/fonts/truetype/dejavu/DejaVuSerif.ttf", 24);
    if (!font) {
        cerr << "Failed to load font: " << TTF_GetError() << "\n";
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        avcodec_free_context(&codecContext);
        avformat_close_input(&formatContext);
        return;
    }

    SDL_Color white = {255, 255, 255, 255};
    ostringstream infoStream;
    infoStream << "Resolution: " << codecParams->width << "x" << codecParams->height << "\n"
               << "Codec: " << avcodec_get_name(codecParams->codec_id) << "\n"
               << "Frame Rate: " << av_q2d(videoStream->avg_frame_rate) << " FPS\n"
               << "Duration: " << (formatContext->duration / AV_TIME_BASE) << " seconds";

    string videoInfo = infoStream.str();
    SDL_Surface* textSurface = TTF_RenderText_Blended_Wrapped(font, videoInfo.c_str(), white, 800);
    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    SDL_Rect textRect = {10, 10, textSurface->w, textSurface->h};
    SDL_FreeSurface(textSurface);

    /* Allocate frames and buffers for decoding and rendering */
    AVFrame* frame = av_frame_alloc();
    AVFrame* frameYUV = av_frame_alloc();
    SwsContext* swsContext = sws_getContext(
        codecParams->width, codecParams->height, codecContext->pix_fmt,
        codecParams->width, codecParams->height, AV_PIX_FMT_YUV420P,
        SWS_BILINEAR, nullptr, nullptr, nullptr);

    int bufferSize = av_image_get_buffer_size(AV_PIX_FMT_YUV420P, codecParams->width, codecParams->height, 1);
    uint8_t* buffer = (uint8_t*)av_malloc(bufferSize);
    av_image_fill_arrays(frameYUV->data, frameYUV->linesize, buffer, AV_PIX_FMT_YUV420P, codecParams->width, codecParams->height, 1);

    /* Read and decode video frames */
    AVPacket* packet = av_packet_alloc();
    SDL_Event event;
    isVideoPlaying = true;

    while (isVideoPlaying) {
        /* Handle SDL events */
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
        /* Check for end of video file or error reading frame */
        if (ret < 0) {
            if (ret == AVERROR_EOF) {
                cout << "End of video file reached. Skipping to next media...\n";
                isVideoPlaying = false;

                SDL_Event event;
                event.type = SDL_USEREVENT;
                event.user.code = 1;
                SDL_PushEvent(&event);
            } else {
                cerr << "Error reading frame: " << ret << "\n";
            }
            break;
        }

        /* Decode and render video frames */
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

    /* Clean up resources */
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
