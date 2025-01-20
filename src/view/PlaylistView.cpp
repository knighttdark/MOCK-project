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
    if (playlists.empty()) {
        std::cout << "No playlists available.\n";
        return;
    }

    // Tạo danh sách playlist để hiển thị
    vector<string> playlist_entries;
    for (size_t i = 0; i < playlists.size(); ++i) {
        const auto& playlist = playlists[i];
        string entry = to_string(i + 1) + " | " + playlist.getName() + " | " + to_string(playlist.getSongs().size()) + " songs";
        playlist_entries.push_back(entry);
    }

    // Biến theo dõi playlist được chọn
    int selected_index = 0;

    // Tạo Menu cho danh sách playlist
    auto playlist_menu = Menu(&playlist_entries, &selected_index);

    // Màn hình Interactive
    auto screen = ScreenInteractive::TerminalOutput();

    // Renderer để hiển thị danh sách playlist
    auto main_renderer = Renderer([&] {
        return vbox({
                   text("=== Playlists ===") | bold | center,
                   separator(),
                   playlist_menu->Render() | border, // Hiển thị menu playlist
                   separator(),
                   text("Use UP/DOWN to navigate, ENTER to select.") | dim | center,
               }) |
               center;
    });

    // Xử lý sự kiện Enter
    auto main_component = CatchEvent(main_renderer, [&](Event event) {
        if (event == Event::Return) { // Khi nhấn Enter
            const auto& selected_playlist = playlists[selected_index];
            std::cout << "Selected Playlist: " << selected_playlist.getName() << " (" << selected_playlist.getSongs().size() << " songs)\n";
            screen.ExitLoopClosure()(); // Thoát màn hình
            return true;
        }

        // Cho phép FTXUI xử lý các sự kiện khác (như mũi tên)
        return playlist_menu->OnEvent(event);
    });

    // Chạy giao diện
    screen.Loop(main_component);
}


// void PlaylistView::displayPlaylists(const vector<Playlist>& playlists) {
//     cout << "=== Playlists ===\n";
//     if (playlists.empty()) {
//         cout << "No playlists available.\n";
//         return;
//     }

//     // Header row
//     cout << "+-----+----------------------+---------------+\n";
//     cout << "| ID  | Playlist Name        | Songs Count   |\n";
//     cout << "+-----+----------------------+---------------+\n";

//     int id = 1; // Playlist ID starts from 1
//     for (const auto& playlist : playlists) {
//         string idStr = to_string(id);
//         string name = playlist.getName();
//         string songCountStr = to_string(playlist.getSongs().size());

//         // Adjust spacing manually
//         cout << "| " << idStr;
//         cout << string(4 - idStr.length(), ' '); // Add spaces to align ID column

//         cout << "| " << name;
//         if (name.length() < 20) {
//             cout << string(20 - name.length(), ' '); // Add spaces to align Playlist Name column
//         }

//         cout << "| " << string(13 - songCountStr.length(), ' ') << songCountStr; // Right-align Songs Count column
//         cout << " |\n";

//         id++;
//     }

//     cout << "+-----+----------------------+---------------+\n";
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