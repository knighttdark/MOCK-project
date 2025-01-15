#ifndef PLAYINGMEDIACONTROLLER_H
#define PLAYINGMEDIACONTROLLER_H

#include "common/BaseController.h"
#include "model/MediaFile.h"
#include <atomic>
#include <mutex>

class PlayingMediaController : public BaseController {
private:
    MediaFile* currentMediaFile = nullptr;
    bool isPlaying = false;
    int volume = 50;
    std::atomic<bool> isUpdating = false;

public:
    void playMediaFile(MediaFile* mediaFile);
    void stop();
    void adjustVolume();
    void handleAction(int action) override;
    // void pause();
    void skipToNext();
    void skipToPrevious();
    void adjustVolume(int level);
    void clearView();
};

#endif // PLAYINGMEDIACONTROLLER_H
