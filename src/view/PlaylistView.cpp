#include "view/PlaylistView.h"
#include <iostream>
#include <iomanip> // For std::setw

void PlaylistView::showMenu() {
    std::cout << "=== Playlist Menu ===\n";
    std::cout << "1. Create a new playlist\n";
    std::cout << "2. Delete a playlist\n";
    std::cout << "3. View a playlist's details\n";
    std::cout << "4. List all playlists\n";
    std::cout << "0. Exit\n";
    std::cout << "Enter your choice: ";
}

int PlaylistView::handleInput() {
    int choice;
    std::cin >> choice;
    return choice;
}

void PlaylistView::displayPlaylists(const std::vector<Playlist>& playlists) {
    std::cout << "=== Playlists ===\n";
    if (playlists.empty()) {
        std::cout << "No playlists available.\n";
        return;
    }

    std::cout << "+-----+----------------------+---------------+\n";
    std::cout << "| ID  | Playlist Name        | Songs Count   |\n";
    std::cout << "+-----+----------------------+---------------+\n";

    int id = 1; // Playlist ID starts from 1
    for (const auto& playlist : playlists) {
        std::cout << "| " << std::setw(3) << id << " | "
                  << std::setw(20) << std::left << playlist.getName() << " | "
                  << std::setw(13) << std::right << playlist.getSongs().size() << " |\n";
        id++;
    }

    std::cout << "+-----+----------------------+---------------+\n";
}

void PlaylistView::displayPlaylistDetails(const Playlist& playlist) {
    std::cout << "=== Playlist Details: " << playlist.getName() << " ===\n";
    const auto& songs = playlist.getSongs();

    if (songs.empty()) {
        std::cout << "No songs in this playlist.\n";
        return;
    }

    // Print table header
    std::cout << "+-----+----------------------+---------------------------+\n";
    std::cout << "| ID  | Song Name            | File Path                |\n";
    std::cout << "+-----+----------------------+---------------------------+\n";

    int songId = 1; // Song ID starts from 1
    for (const auto& song : songs) {
        std::cout << "| " << std::setw(3) << std::left << songId << " | "
                  << std::setw(20) << std::left << song.getName() << " | "
                  << std::setw(25) << std::left << song.getPath() << " |\n";
        songId++;
    }

    std::cout << "+-----+----------------------+---------------------------+\n";
}
