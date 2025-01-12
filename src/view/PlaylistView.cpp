#include "view/PlaylistView.h"
#include "model/Playlist.h"
#include "model/MediaFile.h"
#include <iostream>
#include <iomanip>

void PlaylistView::showMenu() {
    std::cout << "===================================\n";
    std::cout << "|          PLAYLIST MENU          |\n";
    std::cout << "===================================\n";
    std::cout << "| 1. Create Playlist              |\n";
    std::cout << "| 2. View Playlist                |\n";
    std::cout << "| 3. Add Media to Playlist        |\n";
    std::cout << "| 4. Remove Media from Playlist   |\n";
    std::cout << "| 5. Delete Playlist              |\n";
    std::cout << "| 6. Exit                         |\n";
    std::cout << "===================================\n";
    std::cout << "Select an option: ";
}

int PlaylistView::handleInput() {
    int choice;
    std::cin >> choice;
    return choice;
}

void PlaylistView::displayPlaylistDetails(const Playlist& playlist) {
    std::cout << "===================================\n";
    std::cout << "|       PLAYLIST DETAILS          |\n";
    std::cout << "===================================\n";
    std::cout << "Playlist Name: " << playlist.getName() << "\n";
    std::cout << "-----------------------------------\n";

    if (playlist.getMediaFiles().empty()) {
        std::cout << "| No media files in this playlist |\n";
    } else {
        for (const auto& mediaFile : playlist.getMediaFiles()) {
            std::cout << "| - " << mediaFile.getName() << "\n";
        }
    }

    std::cout << "===================================\n";
}
