#ifndef PLAYINGMEDIACONTROLLER_H
#define PLAYINGMEDIACONTROLLER_H

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

class PlayingMediaController : public BaseController {
private:
    // MediaFile* currentMediaFile = nullptr;
    // bool isPlaying = false;
    // int volume = 50;
    // std::atomic<bool> isUpdating = false;

 

    // std::vector<MediaFile>* currentPlaylist = nullptr; // Danh sách bài hát hiện tại
    // size_t currentPlaylistIndex = 0;


    MediaFile* currentMediaFile = nullptr; // Media file currently being played
    std::atomic<bool> isRunning = true;    // Flag to control the playback loop
    bool isPlaying = false;               // Indicates if a media file is being played
    int volume = 50;                      // Volume level (0-100)
    std::atomic<int> currentTime{0}; // Thời gian phát hiện tại
    std::atomic<int> totalTime{0};   // Tổng thời gian của bài hát                // Total playback time in seconds
    std::vector<MediaFile>* currentPlaylist = nullptr; // Playlist of media files
    size_t currentPlaylistIndex = 0;      // Index of the currently playing media in the playlist
    std::atomic<bool> isUpdating;         // Flag to control the update thread
    std::thread updateThread;             // Thread to handle periodic UI updates
    std::mutex mediaMutex;                // Mutex for thread-safe access to shared resources


public:
    // void playMediaFile(MediaFile* mediaFile);
    // void stop();
    // void adjustVolume();
    // void handleAction(int action) override;
    // void skipToNext();
    // void skipToPrevious();
    // void adjustVolume(int level);
    // void clearView();
    // void playPlaylist(std::vector<MediaFile>& playlist);
    // MediaFile* getCurrentMediaFile() const;
    // void updatePlayingView();


    // Methods for controlling playback
    void playMediaFile(MediaFile* mediaFile); // Plays a given media file
    void stop();                              // Stops playback
    void skipToNext();                        // Skips to the next media file in the playlist
    void skipToPrevious();                    // Skips to the previous media file in the playlist
    void playPlaylist(std::vector<MediaFile>& playlist); // Starts playing a playlist

    // Volume control
    void adjustVolume(int level);             // Adjusts the volume level

    // Accessors (Getters)
    MediaFile* getCurrentMediaFile() const;   // Returns the current media file being played
    bool isCurrentlyPlaying() const;         // Checks if a media file is currently playing
    int getCurrentVolume() const;            // Returns the current volume level
    size_t getCurrentPlaylistIndex() const;  // Returns the index of the current media in the playlist
    std::vector<MediaFile>* getCurrentPlaylist() const; // Returns the current playlist

    // Mutators (Setters)
    void setCurrentMediaFile(MediaFile* mediaFile); // Sets the current media file
    void setIsPlaying(bool playing);               // Sets the playback state
    void setCurrentPlaylist(std::vector<MediaFile>* playlist); // Sets the current playlist
    void setCurrentPlaylistIndex(size_t index);    // Sets the index of the current media in the playlist

    bool isRunning_() const;
    
    void handleAction(int action) override;
    void startDisplayLoop();
    void refreshPlayingView();

    int getTotalTime() const;   // Returns the total duration of the media
int getCurrentTime() const; // Returns the current playback time
int getVolume() const;      // Returns the current volume level

    
};

#endif // PLAYINGMEDIACONTROLLER_H

