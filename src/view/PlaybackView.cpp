#include "view/PlaybackView.h"
#include <iostream>

void PlaybackView::showMenu() {
    std::cout << "Playback View Menu" << std::endl;
}

int PlaybackView::handleInput() {
    int choice;
    std::cin >> choice;
    return choice;
}

void PlaybackView::displayPlaybackInfo(const std::string& currentMedia, int duration, int volume) {
    std::cout << "Now playing: " << currentMedia << std::endl;
    std::cout << "Duration: " << duration << " seconds" << std::endl;
    std::cout << "Volume: " << volume << std::endl;
}