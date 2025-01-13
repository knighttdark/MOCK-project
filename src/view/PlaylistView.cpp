#include "view/PlaylistView.h"
#include "common/Exception.h"
#include <iostream>
#include <iomanip> // For setw

void PlaylistView::showMenu() {
    cout << "=== Playlist Menu ===\n";
    cout << "1. Create a new playlist\n";
    cout << "2. Delete a playlist\n";
    cout << "3. View a playlist's details\n";
    cout << "4. List all playlists\n";
    cout << "0. Exit\n";
}

int PlaylistView::handleInput() {
    int choice = Exception::getValidatedInput_Int("Choose an option: ", Exception::validateInputPlaylistView);
    return choice;
}

void PlaylistView::displayPlaylists(const vector<Playlist>& playlists) {
    cout << "=== Playlists ===\n";
    if (playlists.empty()) {
        cout << "No playlists available.\n";
        return;
    }

    cout << "+-----+----------------------+---------------+\n";
    cout << "| ID  | Playlist Name        | Songs Count   |\n";
    cout << "+-----+----------------------+---------------+\n";

    int id = 1; // Playlist ID starts from 1
    for (const auto& playlist : playlists) {
        cout << "| " << setw(3) << id << " | "
                  << setw(20) << left << playlist.getName() << " | "
                  << setw(13) << right << playlist.getSongs().size() << " |\n";
        id++;
    }

    cout << "+-----+----------------------+---------------+\n";
}

void PlaylistView::displayPlaylistDetails(const Playlist& playlist) {
    cout << "=== Playlist Details: " << playlist.getName() << " ===\n";
    const auto& songs = playlist.getSongs();

    if (songs.empty()) {
        cout << "No songs in this playlist.\n";
        return;
    }

    // Print table header
    cout << "+-----+----------------------+---------------------------+\n";
    cout << "| ID  | Song Name            | File Path                |\n";
    cout << "+-----+----------------------+---------------------------+\n";

    int songId = 1; // Song ID starts from 1
    for (const auto& song : songs) {
        cout << "| " << setw(3) << left << songId << " | "
                  << setw(20) << left << song.getName() << " | "
                  << setw(25) << left << song.getPath() << " |\n";
        songId++;
    }

    cout << "+-----+----------------------+---------------------------+\n";
}
