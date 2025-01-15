#include "view/PlayingView.h"
#include <iostream>
#include <iomanip>
#include "common/TerminalUtils.h"



void PlayingView::showMenu() {
    std::cout << "\n=== Playing Menu ===\n";
    std::cout << "1. Play/Pause\n";
    std::cout << "2. Next\n";
    std::cout << "3. Previous\n";
    std::cout << "4. Adjust Volume\n";
    std::cout << "0. Exit\n";
    std::cout << "Enter your choice: ";
    std::cout.flush();
}

int PlayingView::handleInput() {
    int choice;
    std::cin >> choice;
    return choice;
}
void PlayingView::displayPlayingView(const std::string& currentMedia, int duration, int volume, int currentTime) {
    clearTerminal();
    std::cout << "\n=========================================\n";
    std::cout << "              NOW PLAYING                \n";
    std::cout << "=========================================\n";
    std::cout << "Media: " << (!currentMedia.empty() ? currentMedia : "Unknown") << "\n";

    if (duration > 0) {
        int minutes = duration / 60;
        int seconds = duration % 60;
        std::cout << "Duration: " << minutes << " min " << seconds << " sec\n";
    } else {
        std::cout << "Duration: Unknown\n";
    }

    std::cout << "Volume: " << volume << "%\n";

    // Hiển thị thanh tiến trình
    updateProgressBar(currentTime, duration);

    // Hiển thị menu thao tác
    std::cout << "\n\n=== Playing Menu ===\n";
    std::cout << "1. Play/Pause\n";
    std::cout << "2. Next\n";
    std::cout << "3. Previous\n";
    std::cout << "4. Adjust Volume\n";
    std::cout << "0. Exit\n";
    std::cout << "Enter your choice: ";
    std::cout.flush();
}


void PlayingView::updateProgressBar(int currentTime, int duration) {
    int progressWidth = 30; // Chiều rộng của thanh tiến trình
    int progress = (duration > 0) ? (currentTime * progressWidth / duration) : 0;

    int currentMinutes = currentTime / 60;
    int currentSeconds = currentTime % 60;
    int totalMinutes = duration / 60;
    int totalSeconds = duration % 60;

    std::cout << "\rProgress: [";
    for (int i = 0; i < progressWidth; ++i) {
        if (i < progress) {
            std::cout << "#";
        } else {
            std::cout << "-";
        }
    }
    std::cout << "] " << std::setfill('0') << std::setw(2) << currentMinutes << ":"
              << std::setfill('0') << std::setw(2) << currentSeconds << " / "
              << std::setfill('0') << std::setw(2) << totalMinutes << ":"
              << std::setfill('0') << std::setw(2) << totalSeconds << "   ";
    std::cout.flush();
}

void PlayingView::clearView() {
    system("clear"); // For Linux/MacOS
    // system("cls"); // For Windows
}