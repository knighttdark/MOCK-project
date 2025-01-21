#ifndef PLAYINGMEDIACONTROLLER_H
#define PLAYINGMEDIACONTROLLER_H

extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libavutil/avutil.h>
#include <libavutil/imgutils.h>
#include <libavutil/opt.h>
#include <libavutil/time.h>
#include <libswscale/swscale.h>
#include <libswresample/swresample.h>
}
#include <SDL2/SDL.h>
#include <SDL2/SDL_audio.h>
#include <SDL2/SDL_mixer.h>

#include "common/BaseController.h"
#include "model/MediaFile.h"
#include <atomic>
#include <mutex>
#include "controller/PlayingMediaController.h"
#include "controller/ManagerController.h"
#include "model/Metadata.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <iostream>
#include <taglib/fileref.h>
#include <controller/MediaFileController.h>
#include <bits/this_thread_sleep.h>
#include <thread>
#include <condition_variable>
#include <common/Enum.h>

class PlayingMediaController : public BaseController {
private:
    MediaFile* currentMediaFile = nullptr;
    std::atomic<bool> isRunning = true;
    bool isPlaying = false;
    int volume = 50;
    std::atomic<int> currentTime{0};
    std::atomic<int> totalTime{0};
    std::vector<MediaFile>* currentPlaylist = nullptr;
    size_t currentPlaylistIndex = 0;
    std::atomic<bool> isUpdating;
    std::thread updateThread;
    std::mutex mediaMutex;
public:
    void playMediaFile(MediaFile* mediaFile);
    MediaFile* getCurrentMediaFile() const;
    bool isCurrentlyPlaying() const;
    int getCurrentVolume() const;
    size_t getCurrentPlaylistIndex() const;
    std::vector<MediaFile>* getCurrentPlaylist() const;

    void setCurrentMediaFile(MediaFile* mediaFile);
    void setIsPlaying(bool playing);
    void setCurrentPlaylist(std::vector<MediaFile>* playlist);
    void setCurrentPlaylistIndex(size_t index);
    void stop();
    bool isRunning_() const;
    void handleAction(int action) override;
    void startDisplayLoop();
    void refreshPlayingView();
    int getTotalTime() const;
    int getCurrentTime() const;
    int getVolume() const;
    void skipToNext();
    void skipToPrevious();
    void adjustVolume(int level);
    void clearView();
    void playPlaylist(std::vector<MediaFile>& playlist);
    void playVideo(const std::string& videoPath);
};

 #endif
