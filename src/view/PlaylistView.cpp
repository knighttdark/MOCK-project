#include "view/PlaylistView.h"
#include "common/Exception.h"
#include <iostream>
#include <iomanip> // For setw

int PlaylistView::showMenu() {
    MenuRenderer menu("Playlist Menu", 
                      {"1. Create a new playlist", "2. Delete a playlist", 
                       "3. View playlist details", "4. List all playlists", 
                       "5. Play Songs", "0. Exit"},
                      {1, 2, 3, 4, 5, 0});
    return menu.render();
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
