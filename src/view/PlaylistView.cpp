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

const string& PlaylistView::getSelectedPath() const {
    return selectedPath;
}


void PlaylistView::setSelectedPath(const string& path) {
    selectedPath = path;
}


int PlaylistView::getSelectedPlaylistID() const {
        return selected_playlist_ID;
    }

void PlaylistView::setSelectedPlaylistID(int id) {
    selected_playlist_ID = id;
}

const string& PlaylistView::getSelectedPlaylistName() const {
        return selected_playlist_name;
}
void PlaylistView::setSelectedPlaylistName( string name) {
        selected_playlist_name = name;
}

void PlaylistView::displayPlaylists(const vector<Playlist>& playlists) {
    if (playlists.empty()) {
        std::cout << "No playlists available.\n";
        return;
    }

    vector<string> playlist_entries;
    for (size_t i = 0; i < playlists.size(); ++i) {
        const auto& playlist = playlists[i];

        string id_column = to_string(i + 1);
        string name_column = playlist.getName();
        string song_count_column = to_string(playlist.getSongs().size()) + " songs";

        id_column += string(4 - id_column.length(), ' ');
        name_column += string(20 - name_column.length(), ' ');
        song_count_column = string(10 - song_count_column.length(), ' ') + song_count_column;

        string entry = id_column + " | " + name_column + " | " + song_count_column;
        playlist_entries.push_back(entry);
    }

    int selected_index = 0;

    auto playlist_menu = Menu(&playlist_entries, &selected_index);

    auto screen = ScreenInteractive::TerminalOutput();

    auto main_renderer = Renderer([&] {
        return vbox({
                   text("=== Playlists ===") | bold | center,
                   separator(),
                   playlist_menu->Render() | border, 
                   separator(),
                   text("Use UP/DOWN or click to navigate, ENTER to select.") | dim | center,
               }) |
               center;
    });

    auto main_component = CatchEvent(main_renderer, [&](Event event) {
    if (event == Event::Return) {
        if (selected_index >= 0 && selected_index < (int)playlists.size()) {
            setSelectedPlaylistID(selected_index + 1);
            screen.ExitLoopClosure()();
        }
        return true;
    }

    if (event.is_mouse() && event.mouse().button == Mouse::Left && event.mouse().motion == Mouse::Pressed) {
        int clicked_index = event.mouse().y - 3; 
        if (clicked_index >= 0 && clicked_index < (int)playlist_entries.size()) {
            selected_index = clicked_index;
            setSelectedPlaylistID(selected_index + 1);
            screen.ExitLoopClosure()();
        }
        return true;
    }

    return playlist_menu->OnEvent(event);
    });

    screen.Loop(main_component);
}

void PlaylistView::displayPlaylistDetails(const Playlist& playlist) {
    const auto& songs = playlist.getSongs();

    if (songs.empty()) {
        auto empty_renderer = Renderer([] {
            return vbox({
                text("No songs in this playlist.") | bold | center,
                separator(),
                text("Press ENTER to return.") | dim | center
            }) | center;
        });

        auto screen = ScreenInteractive::TerminalOutput();
        auto main_component = CatchEvent(empty_renderer, [&](Event event) {
            if (event == Event::Return) {
                screen.ExitLoopClosure()();
                return true;
            }
            return false;
        });

        screen.Loop(main_component);
        return;
    }

    std::vector<std::string> menu_entries;
    for (const auto& song : songs) {
        menu_entries.push_back(song.getName());
    }

    int selected = 0;
    auto menu = Menu(&menu_entries, &selected);

    std::string currentPath;

    auto renderer = Renderer(menu, [&] {
        return vbox({
            text("=== Playlist Details: " + playlist.getName() + " ===") | bold | center,
            separator(),
            menu->Render() | border | center,
            separator(),
            text("Path: " + currentPath) | dim | center, 
            text("Use Arrow Keys to navigate, ENTER to select.") | dim | center
        }) | center;
    });

    auto screen = ScreenInteractive::TerminalOutput();
    auto main_component = CatchEvent(renderer, [&](Event event) {

        if (event == Event::Return) {
            if (selected >= 0 && selected < (int)songs.size()) {
                setSelectedPath(songs[selected].getPath());
                std::cout << "Selected song path: " << currentPath << std::endl;
                screen.ExitLoopClosure()();
            }
            return true;
        }

        if (event.is_mouse() && event.mouse().button == Mouse::Left && event.mouse().motion == Mouse::Pressed) {
            int clicked_index = event.mouse().y - 3;
            if (clicked_index >= 0 && clicked_index < (int)songs.size()) {
                selected = clicked_index;
                setSelectedPath(songs[selected].getPath());
            }
            return true;
        }

        if (event == Event::ArrowUp || event == Event::ArrowDown) {
            menu->OnEvent(event);
            currentPath = songs[selected].getPath();
            return true;
        }

        return false;
    });

    screen.Loop(main_component);
}



