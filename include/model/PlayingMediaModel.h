#ifndef PLAYINGMEDIAMODEL_H
#define PLAYINGMEDIAMODEL_H

#include <atomic>
#include <vector>
#include "model/MediaFile.h"
#include <SDL2/SDL_mixer.h>

class PlayingMediaModel {
private:
    std::atomic<bool> isRunning;
    std::atomic<bool> isPlaying;
    MediaFile* currentMediaFile;
    Mix_Music* currentMusic;
    int volume;
    int currentTime;
    int duration;

public:
    PlayingMediaModel();
    ~PlayingMediaModel();

    // Getters and Setters
    bool getIsRunning() const;
    void setIsRunning(bool running);

    bool getIsPlaying() const;
    void setIsPlaying(bool playing);

    MediaFile* getCurrentMediaFile() const;
    void setCurrentMediaFile(MediaFile* mediaFile);

    Mix_Music* getCurrentMusic() const;
    void setCurrentMusic(Mix_Music* music);

    int getVolume() const;
    void setVolume(int vol);

    int getCurrentTime() const;
    void setCurrentTime(int time);

    int getDuration() const;
    void setDuration(int dur);

    void reset();
};

#endif // PLAYINGMEDIAMODEL_H
