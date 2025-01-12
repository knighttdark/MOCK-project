#include "view/PlaybackView.h"
#include <iostream>

void PlaybackView::showMenu() {
    cout << "Playback View Menu" << endl;
}

int PlaybackView::handleInput() {
    int choice;
    cin >> choice;
    return choice;
}

void PlaybackView::displayPlaybackInfo(const string& currentMedia, int duration, int volume) {
    cout << "Now playing: " << currentMedia << endl;
    cout << "Duration: " << duration << " seconds" << endl;
    cout << "Volume: " << volume << endl;
}