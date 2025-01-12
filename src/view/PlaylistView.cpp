#include "view/PlaylistView.h"
#include <iostream>

void PlaylistView::showMenu() {
    cout << "Playlist View Menu" << endl;
}

int PlaylistView::handleInput() {
    int choice;
    cin >> choice;
    return choice;
}

void PlaylistView::displayPlaylists(const vector<string>& playlists) {
    cout << "Available Playlists:" << endl;
    for (const auto& playlist : playlists) {
        cout << playlist << endl;
    }
}

void PlaylistView::displayPlaylistDetails(const string& playlist) {
    cout << "Details of playlist: " << playlist << endl;
}
