#include "view/PlayingView.h"
#include <iostream>
#include <iomanip>
#include "common/TerminalUtils.h"
#include "common/Exception.h"



// void PlayingView::showMenu() {
//     cout << "\n=== Playing Menu ===\n";
//     cout << "1. Play/Pause\n";
//     cout << "2. Next\n";
//     cout << "3. Previous\n";
//     cout << "4. Adjust Volume\n";
//     cout << "0. Exit\n";
//     cout << "Enter your choice: ";
//     cout.flush();
// }

// int PlayingView::handleInput() {
//     int choice;
//     cin >> choice;
//     return choice;
// }

int PlayingView::showMenu() {
    cout << "\n=== Playing Menu ===\n";
    cout << "1. Play/Pause\n";
    cout << "2. Next\n";
    cout << "3. Previous\n";
    cout << "4. Adjust Volume\n";
    cout << "5. Stop\n";
    cout << "0. Exit\n";
    cout << "Enter your choice: ";
    cout.flush();

    int choice = Exception::getValidatedInput_Int("Choose an option: ", Exception::validateInputPlayingView);

    return choice;
}


    void PlayingView::displayPlayingView(const string& currentMedia, int duration, int volume, int currentTime) {
        clearTerminal();
        cout << "\n=========================================\n";
        cout << "              NOW PLAYING                \n";
        cout << "=========================================\n";
        cout << "Media: " << (!currentMedia.empty() ? currentMedia : "Unknown") << "\n";

        if (duration > 0) {
            int minutes = duration / 60;
            int seconds = duration % 60;
            cout << "Duration: " << minutes << " min " << seconds << " sec\n";
        } else {
            cout << "Duration: Unknown\n";
        }

        cout << "Volume: " << volume << "%\n";

        // Hiển thị thanh tiến trình
        updateProgressBar(currentTime, duration);

        //Hiển thị menu thao tác
        cout << "\n\n=== Playing Menu ===\n";
        cout << "1. Play/Pause\n";
        cout << "2. Next\n";
        cout << "3. Previous\n";
        cout << "4. Adjust Volume\n";
        cout << "5. Stop\n";
        cout << "0. Exit\n";
        cout << "Enter your choice: ";
        cout.flush();
    }



void PlayingView::updateProgressBar(int currentTime, int duration) {
    int progressWidth = 30; // Chiều rộng của thanh tiến trình
    int progress = (duration > 0) ? (currentTime * progressWidth / duration) : 0;

    int currentMinutes = currentTime / 60;
    int currentSeconds = currentTime % 60;
    int totalMinutes = duration / 60;
    int totalSeconds = duration % 60;

    cout << "\rProgress: [";
    for (int i = 0; i < progressWidth; ++i) {
        if (i < progress) {
            cout << "#";
        } else {
            cout << "-";
        }
    }
    cout << "] " << setfill('0') << setw(2) << currentMinutes << ":"
              << setfill('0') << setw(2) << currentSeconds << " / "
              << setfill('0') << setw(2) << totalMinutes << ":"
              << setfill('0') << setw(2) << totalSeconds << "   ";
    cout.flush();
}

void PlayingView::clearView() {
    system("clear"); // For Linux/MacOS
    // system("cls"); // For Windows
}