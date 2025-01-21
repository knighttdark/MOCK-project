
#include "controller/PlayingMediaController.h"
#include "controller/PlayingMediaController.h"
#include "view/PlayingView.h"
#include "common/Enum.h"
#include "common/TerminalUtils.h"

bool isSDLInitialized = false;
Mix_Music* currentMusic = nullptr;

void initializeSDL() {
    if (SDL_Init(SDL_INIT_AUDIO) < 0) {
        cerr << "Failed to initialize SDL: " << SDL_GetError() << "\n";
        return;
    }
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        cerr << "Failed to initialize SDL_mixer: " << Mix_GetError() << "\n";
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
            cout << "Enter new volume (0-100): ";
            cin >> newVolume;
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
        cerr << "Volume level must be between 0 and 100.\n";
        return;
    }

    volume = level;
    Mix_VolumeMusic((MIX_MAX_VOLUME * volume) / 100);

    
}


void PlayingMediaController::stop() {
    if (!isPlaying) return;

    isRunning = false;

    
        if (updateThread.joinable() && this_thread::get_id() != updateThread.get_id()) {
        updateThread.join();
    }

    if (Mix_PlayingMusic()) {
        Mix_HaltMusic();
    }

    if (currentMusic) {
        Mix_FreeMusic(currentMusic);
        currentMusic = nullptr;
    }

    PlayingView* playingView = dynamic_cast<PlayingView*>(
        ManagerController::getInstance().getManagerView()->getView());
    if (playingView) {
        playingView->clearView();
        playingView->displayPlayingView("No Media", 0, volume, 0);
    }

    isPlaying = false;
}




void PlayingMediaController::skipToNext() {
    stop(); 

    if (currentPlaylist && !currentPlaylist->empty()) {
        currentPlaylistIndex = (currentPlaylistIndex + 1) % currentPlaylist->size();
        currentMediaFile = &(*currentPlaylist)[currentPlaylistIndex];
    } else {
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

    playMediaFile(currentMediaFile); 
}

void PlayingMediaController::playMediaFile(MediaFile* mediaFile) {
    if (!mediaFile) {
        cerr << "No media file provided.\n";
        return;
    }

    stop(); 
    isRunning = true; 

    if (!isSDLInitialized) {
        initializeSDL();
        if (!isSDLInitialized) {
            cerr << "Failed to initialize SDL.\n";
            return;
        }
    }

    currentMusic = Mix_LoadMUS(mediaFile->getPath().c_str());
    if (!currentMusic) {
        cerr << "Failed to load music file: " << mediaFile->getPath() << " (" << Mix_GetError() << ")\n";
        return;
    }

    if (Mix_PlayMusic(currentMusic, 1) == -1) {
        cerr << "Failed to play music: " << Mix_GetError() << "\n";
        return;
    }

    currentMediaFile = mediaFile;
    isPlaying = true;

    auto fileRef = TagLib::FileRef(mediaFile->getPath().c_str());
    totalTime = fileRef.audioProperties() ? fileRef.audioProperties()->length() : 0;
    currentTime = 0;

    
    startDisplayLoop();
}
void PlayingMediaController::startDisplayLoop() {
    if (updateThread.joinable()) {
        cout << "Display loop is already running. Ensuring updates...\n";
        isRunning = true; 
        return;
    }

    PlayingView* playingView = dynamic_cast<PlayingView*>(
        ManagerController::getInstance().getManagerView()->getView());
    if (!playingView) {
        cerr << "PlayingView not available.\n";
        return;
    }

    updateThread = thread([this, playingView]() {
        while (isRunning) {
            {
                lock_guard<mutex> lock(mediaMutex);
                playingView->clearView();
                playingView->displayPlayingView(
                    currentMediaFile ? currentMediaFile->getName() : "No Media",
                    totalTime.load(),
                    volume,
                    currentTime.load()
                );
            }

            this_thread::sleep_for(chrono::seconds(1)); 

            if (isPlaying) {
                ++currentTime;
                if (currentTime >= totalTime) {
                    cout << "\nPlayback finished. Skipping to next track...\n";
                    isRunning = false; 
                    skipToNext();
                }
            }
        }
    });
}

void PlayingMediaController::playPlaylist(vector<MediaFile>& playlist) {
    currentPlaylist = &playlist;
    currentPlaylistIndex = 0;
    if (!playlist.empty()) {
        currentMediaFile = &playlist[currentPlaylistIndex];
        playMediaFile(currentMediaFile);
    } else {
        cerr << "Playlist is empty.\n";
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

vector<MediaFile>* PlayingMediaController::getCurrentPlaylist() const {
    return currentPlaylist;
}
void PlayingMediaController::setCurrentMediaFile(MediaFile* mediaFile) {
    lock_guard<mutex> lock(mediaMutex);
    currentMediaFile = mediaFile;
}

void PlayingMediaController::setIsPlaying(bool playing) {
    lock_guard<mutex> lock(mediaMutex);
    isPlaying = playing;
}

void PlayingMediaController::setCurrentPlaylist(vector<MediaFile>* playlist) {
    lock_guard<mutex> lock(mediaMutex);
    currentPlaylist = playlist;
}

void PlayingMediaController::setCurrentPlaylistIndex(size_t index) {
    lock_guard<mutex> lock(mediaMutex);
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
