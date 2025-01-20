#include "view/PlaylistView.h"
#include "common/Exception.h"
#include <iostream>
#include <iomanip> 

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

    int id = 1; 
    for (const auto& playlist : playlists) {
        string idStr = to_string(id);
        string name = playlist.getName();
        string songCountStr = to_string(playlist.getSongs().size());

        
        cout << "| " << idStr;
        cout << string(4 - idStr.length(), ' '); 

        cout << "| " << name;
        if (name.length() < 20) {
            cout << string(20 - name.length(), ' '); 
        }

        cout << "| " << string(13 - songCountStr.length(), ' ') << songCountStr; 
        cout << " |\n";

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

    
    size_t maxIdWidth = 2; 
    size_t maxNameWidth = 10; 
    size_t maxPathWidth = 10; 

    for (size_t i = 0; i < songs.size(); ++i) {
        maxIdWidth = max(maxIdWidth, to_string(i + 1).length());
        maxNameWidth = max(maxNameWidth, songs[i].getName().length());
        maxPathWidth = max(maxPathWidth, songs[i].getPath().length());
    }

    
    maxIdWidth += 2;
    maxNameWidth += 2;
    maxPathWidth += 2;

    
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

    
    for (size_t i = 0; i < songs.size(); ++i) {
        string idStr = to_string(i + 1);
        string songName = songs[i].getName();
        string filePath = songs[i].getPath();

        cout << "| " << idStr << string(maxIdWidth - idStr.length() - 1, ' ')
             << "| " << songName << string(maxNameWidth - songName.length() - 1, ' ')
             << "| " << filePath << string(maxPathWidth - filePath.length() - 1, ' ')
             << " |\n";
    }

    
    cout << "+" << string(maxIdWidth, '-') << "+" 
         << string(maxNameWidth, '-') << "+" 
         << string(maxPathWidth, '-') << "+\n";
}