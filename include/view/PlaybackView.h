#ifndef PLAYBACKVIEW_H
#define PLAYBACKVIEW_H

#include "common/BaseView.h"
#include <string>

class PlaybackView : public BaseView {
public:
    void showMenu() override;
    int handleInput() override;
    void displayPlaybackInfo(const std::string& currentMedia, int duration, int volume);
};

#endif // PLAYBACKVIEW_HPP
