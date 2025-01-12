#include "view/PlaylistView.h"
#include "common/exception.h"
#include <iostream>

void PlaylistView::showMenu() {
    cout << "Playlist View Menu" << endl;
}

int PlaylistView::handleInput() {
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

void PlaylistView::displayPlaylists(const vector<string>& playlists) {
    cout << "Available Playlists:" << endl;
    for (const auto& playlist : playlists) {
        cout << playlist << endl;
    }
}

void PlaylistView::displayPlaylistDetails(const string& playlist) {
    cout << "Details of playlist: " << playlist << endl;
}
