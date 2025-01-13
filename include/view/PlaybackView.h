#ifndef PLAYBACKVIEW_H
#define PLAYBACKVIEW_H

#include "common/BaseView.h"
#include <string>

/* View for playback controls */
class PlaybackView : public BaseView {
public:
    void showMenu() override; /* Show menu */
    int handleInput() override; /* Handle input */
    void displayPlaybackInfo(const string& currentMedia, int duration, int volume); /* Display playback info */
};

#endif /* PLAYBACKVIEW_H */