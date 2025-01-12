#include "view/PlaybackView.h"
#include "common/exception.h"
#include <iostream>

void PlaybackView::showMenu() {
    cout << "Playback View Menu" << endl;
}

int PlaybackView::handleInput() {
    int choice;
    while (true) {
        try {
            cout << "\nEnter your choice: ";
            cin >> choice;

            if (cin.fail()) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                throw InputException("Invalid input. Please enter a valid number.");
            }

            return choice;
        } catch (const InputException& e) {
            cerr << "Error: " << e.what() << endl;
        }
    }
}

// int PlaybackView::handleInput() {
//     int choice;
//     cin >> choice;
//     return choice;
// }

void PlaybackView::displayPlaybackInfo(const string& currentMedia, int duration, int volume) {
    cout << "Now playing: " << currentMedia << endl;
    cout << "Duration: " << duration << " seconds" << endl;
    cout << "Volume: " << volume << endl;
}