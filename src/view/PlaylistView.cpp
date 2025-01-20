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


// void PlaylistView::displayPlaylists(const vector<Playlist>& playlists) {
//     cout << "=== Playlists ===\n";
//     if (playlists.empty()) {
//         cout << "No playlists available.\n";
//         return;
//     }

//     cout << "+-----+----------------------+---------------+\n";
//     cout << "| ID  | Playlist Name        | Songs Count   |\n";
//     cout << "+-----+----------------------+---------------+\n";

//     int id = 1; // Playlist ID starts from 1
//     for (const auto& playlist : playlists) {
//         cout << "| " << setw(3) << id << " | "
//                   << setw(20) << left << playlist.getName() << " | "
//                   << setw(13) << right << playlist.getSongs().size() << " |\n";
//         id++;
//     }

//     cout << "+-----+----------------------+---------------+\n";
// }
void PlaylistView::displayPlaylists(const vector<Playlist>& playlists) {
    cout << "=== Playlists ===\n";
    if (playlists.empty()) {
        cout << "No playlists available.\n";
        return;
    }

    // Header row
    cout << "+-----+----------------------+---------------+\n";
    cout << "| ID  | Playlist Name        | Songs Count   |\n";
    cout << "+-----+----------------------+---------------+\n";

    int id = 1; // Playlist ID starts from 1
    for (const auto& playlist : playlists) {
        string idStr = to_string(id);
        string name = playlist.getName();
        string songCountStr = to_string(playlist.getSongs().size());

        // Adjust spacing manually
        cout << "| " << idStr;
        cout << string(4 - idStr.length(), ' '); // Add spaces to align ID column

        cout << "| " << name;
        if (name.length() < 20) {
            cout << string(20 - name.length(), ' '); // Add spaces to align Playlist Name column
        }

        cout << "| " << string(13 - songCountStr.length(), ' ') << songCountStr; // Right-align Songs Count column
        cout << " |\n";

        id++;
    }

    cout << "+-----+----------------------+---------------+\n";
}


// void PlaylistView::displayPlaylistDetails(const Playlist& playlist) {
//     cout << "=== Playlist Details: " << playlist.getName() << " ===\n";
//     const auto& songs = playlist.getSongs();

//     if (songs.empty()) {
//         cout << "No songs in this playlist.\n";
//         return;
//     }

//     // Print table header
//     cout << "+-----+----------------------+---------------------------+\n";
//     cout << "| ID  | Song Name            | File Path                |\n";
//     cout << "+-----+----------------------+---------------------------+\n";

//     int songId = 1; // Song ID starts from 1
//     for (const auto& song : songs) {
//         cout << "| " << setw(3) << left << songId << " | "
//                   << setw(20) << left << song.getName() << " | "
//                   << setw(25) << left << song.getPath() << " |\n";
//         songId++;
//     }

//     cout << "+-----+----------------------+---------------------------+\n";
// }
// void PlaylistView::displayPlaylistDetails(const Playlist& playlist) {
//     cout << "=== Playlist Details: " << playlist.getName() << " ===\n";
//     const auto& songs = playlist.getSongs();

//     if (songs.empty()) {
//         cout << "No songs in this playlist.\n";
//         return;
//     }

//     // Print table header
//     cout << "+-----+----------------------+---------------------------+\n";
//     cout << "| ID  | Song Name            | File Path                |\n";
//     cout << "+-----+----------------------+---------------------------+\n";

//     int songId = 1; // Song ID starts from 1
//     for (const auto& song : songs) {
//         string idStr = to_string(songId);
//         string songName = song.getName();
//         string filePath = song.getPath();

//         // Print ID column
//         cout << "| " << idStr;
//         cout << string(4 - idStr.length(), ' '); // Add spaces to align ID column

//         // Print Song Name column
//         cout << "| " << songName;
//         if (songName.length() < 20) {
//             cout << string(20 - songName.length(), ' '); // Add spaces to align Song Name column
//         }

//         // Print File Path column
//         cout << "| " << filePath;
//         if (filePath.length() < 25) {
//             cout << string(25 - filePath.length(), ' '); // Add spaces to align File Path column
//         }

//         // End row
//         cout << " |\n";
//         songId++;
//     }

//     // Print footer
//     cout << "+-----+----------------------+---------------------------+\n";
// }
void PlaylistView::displayPlaylistDetails(const Playlist& playlist) {
    cout << "=== Playlist Details: " << playlist.getName() << " ===\n";
    const auto& songs = playlist.getSongs();

    if (songs.empty()) {
        cout << "No songs in this playlist.\n";
        return;
    }

    // Determine column widths dynamically
    size_t maxIdWidth = 2; // Minimum width for ID
    size_t maxNameWidth = 10; // Minimum width for Song Name
    size_t maxPathWidth = 10; // Minimum width for File Path

    for (size_t i = 0; i < songs.size(); ++i) {
        maxIdWidth = std::max(maxIdWidth, std::to_string(i + 1).length());
        maxNameWidth = std::max(maxNameWidth, songs[i].getName().length());
        maxPathWidth = std::max(maxPathWidth, songs[i].getPath().length());
    }

    // Add padding for better readability
    maxIdWidth += 2;
    maxNameWidth += 2;
    maxPathWidth += 2;

    // Print table header
    cout << "+" << string(maxIdWidth, '-') << "+" 
         << string(maxNameWidth, '-') << "+" 
         << string(maxPathWidth, '-') << "+\n";
    cout << "| " << "ID" << string(maxIdWidth - 3, ' ') 
         << "| " << "Song Name" << string(maxNameWidth - 10, ' ') 
         << "| " << "File Path" << string(maxPathWidth - 10, ' ') 
         << " |\n";
    cout << "+" << string(maxIdWidth, '-') << "+" 
         << string(maxNameWidth, '-') << "+" 
         << string(maxPathWidth, '-') << "+\n";

    // Print table rows
    for (size_t i = 0; i < songs.size(); ++i) {
        string idStr = to_string(i + 1);
        string songName = songs[i].getName();
        string filePath = songs[i].getPath();

        cout << "| " << idStr << string(maxIdWidth - idStr.length() - 1, ' ')
             << "| " << songName << string(maxNameWidth - songName.length() - 1, ' ')
             << "| " << filePath << string(maxPathWidth - filePath.length() - 1, ' ')
             << " |\n";
    }

    // Print footer
    cout << "+" << string(maxIdWidth, '-') << "+" 
         << string(maxNameWidth, '-') << "+" 
         << string(maxPathWidth, '-') << "+\n";
}