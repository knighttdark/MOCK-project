#include "view/PlaylistView.h"
#include <iostream>

void PlaylistView::showMenu() {
    std::cout << "Playlist View Menu" << std::endl;
}

int PlaylistView::handleInput() {
    int choice;
    std::cin >> choice;
    return choice;
}

void PlaylistView::displayPlaylists(const std::vector<std::string>& playlists) {
    std::cout << "Available Playlists:" << std::endl;
    for (const auto& playlist : playlists) {
        std::cout << playlist << std::endl;
    }
}

void PlaylistView::displayPlaylistDetails(const std::string& playlist) {
    std::cout << "Details of playlist: " << playlist << std::endl;
}
