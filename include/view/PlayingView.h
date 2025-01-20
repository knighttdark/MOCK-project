#ifndef PLAYINGVIEW_H
#define PLAYINGVIEW_H

#include "common/BaseView.h"
#include <string>

class PlayingView : public BaseView {
public:
    // void showMenu() override;
    // int handleInput() override;
    int showMenu() override;
    void displayPlayingView(const string& currentMedia, int duration, int volume, int currentTime);
    void updateProgressBar(int currentTime, int duration);

    void clearView();
};

#endif // PLAYINGVIEW_H
