#include "model/PlayingMediaModel.h"

PlayingMediaModel::PlayingMediaModel()
    : isRunning(true), isPlaying(false), currentMediaFile(nullptr),
      currentMusic(nullptr), volume(50), currentTime(0), duration(0) {}

PlayingMediaModel::~PlayingMediaModel() {
    if (currentMusic) {
        Mix_FreeMusic(currentMusic);
    }
}

bool PlayingMediaModel::getIsRunning() const {
    return isRunning;
}

void PlayingMediaModel::setIsRunning(bool running) {
    isRunning = running;
}

bool PlayingMediaModel::getIsPlaying() const {
    return isPlaying;
}

void PlayingMediaModel::setIsPlaying(bool playing) {
    isPlaying = playing;
}

MediaFile* PlayingMediaModel::getCurrentMediaFile() const {
    return currentMediaFile;
}

void PlayingMediaModel::setCurrentMediaFile(MediaFile* mediaFile) {
    currentMediaFile = mediaFile;
}

Mix_Music* PlayingMediaModel::getCurrentMusic() const {
    return currentMusic;
}

void PlayingMediaModel::setCurrentMusic(Mix_Music* music) {
    currentMusic = music;
}

int PlayingMediaModel::getVolume() const {
    return volume;
}

void PlayingMediaModel::setVolume(int vol) {
    volume = vol;
}

int PlayingMediaModel::getCurrentTime() const {
    return currentTime;
}

void PlayingMediaModel::setCurrentTime(int time) {
    currentTime = time;
}

int PlayingMediaModel::getDuration() const {
    return duration;
}

void PlayingMediaModel::setDuration(int dur) {
    duration = dur;
}

void PlayingMediaModel::reset() {
    if (currentMusic) {
        Mix_HaltMusic();
        Mix_FreeMusic(currentMusic);
        currentMusic = nullptr;
    }
    currentMediaFile = nullptr;
    currentTime = 0;
    duration = 0;
    isPlaying = false;
}
