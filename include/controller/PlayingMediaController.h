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

 

    std::vector<MediaFile>* currentPlaylist = nullptr; // Danh sách bài hát hiện tại
    size_t currentPlaylistIndex = 0;

public:
    void playMediaFile(MediaFile* mediaFile);
    void stop();
    void adjustVolume();
    void handleAction(int action) override;
    void skipToNext();
    void skipToPrevious();
    void adjustVolume(int level);
    void clearView();
    void playPlaylist(std::vector<MediaFile>& playlist);
    
};

#endif // PLAYINGMEDIACONTROLLER_H
