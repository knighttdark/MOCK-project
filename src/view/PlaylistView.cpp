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

    // Tạo danh sách playlist để hiển thị với định dạng cột
    vector<string> playlist_entries;
    for (size_t i = 0; i < playlists.size(); ++i) {
        const auto& playlist = playlists[i];

        // Căn chỉnh cột với chiều rộng cố định
        string id_column = to_string(i + 1);               // ID
        string name_column = playlist.getName();          // Playlist name
        string song_count_column = to_string(playlist.getSongs().size()) + " songs"; // Song count

        // Đảm bảo các cột thẳng hàng
        id_column += string(4 - id_column.length(), ' ');             // Cột ID rộng 4
        name_column += string(20 - name_column.length(), ' ');        // Cột Name rộng 20
        song_count_column = string(10 - song_count_column.length(), ' ') + song_count_column; // Cột Count rộng 10, căn phải

        // Gộp lại thành một dòng hoàn chỉnh
        string entry = id_column + " | " + name_column + " | " + song_count_column;
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
                   text("Use UP/DOWN or click to navigate, ENTER to select.") | dim | center,
               }) |
               center;
    });

    // Xử lý sự kiện nhập từ bàn phím và chuột
auto main_component = CatchEvent(main_renderer, [&](Event event) {
    // Xử lý khi nhấn Enter
    if (event == Event::Return) {
        if (selected_index >= 0 && selected_index < (int)playlists.size()) {
            setSelectedPlaylistID(selected_index + 1);
            //setSelectedPlaylistName(playlists[selected_index].getName());
            screen.ExitLoopClosure()();
        }
        return true;
    }

    // Xử lý sự kiện click chuột
    if (event.is_mouse() && event.mouse().button == Mouse::Left && event.mouse().motion == Mouse::Pressed) {
        int clicked_index = event.mouse().y - 3; // Điều chỉnh vị trí menu
        if (clicked_index >= 0 && clicked_index < (int)playlist_entries.size()) {
            selected_index = clicked_index;
            setSelectedPlaylistID(selected_index + 1);
            //setSelectedPlaylistName(playlists[selected_index].getName());
            screen.ExitLoopClosure()();
        }
        return true;
    }

    // Xử lý các sự kiện khác (mũi tên)
    return playlist_menu->OnEvent(event);
});
    // Chạy giao diện
    screen.Loop(main_component);
}

void PlaylistView::displayPlaylistDetails(const Playlist& playlist) {
    const auto& songs = playlist.getSongs();

    // Nếu playlist không có bài hát
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

    // Xác định độ rộng các cột
    size_t maxIdWidth = 2;     // Độ rộng tối thiểu cho cột ID
    size_t maxNameWidth = 10;  // Độ rộng tối thiểu cho tên bài hát
    size_t maxPathWidth = 10;  // Độ rộng tối thiểu cho đường dẫn file

    for (size_t i = 0; i < songs.size(); ++i) {
        maxIdWidth = std::max(maxIdWidth, std::to_string(i + 1).length());
        maxNameWidth = std::max(maxNameWidth, songs[i].getName().length());
        maxPathWidth = std::max(maxPathWidth, songs[i].getPath().length());
    }

    // Thêm padding cho dễ đọc
    maxIdWidth += 2;
    maxNameWidth += 2;
    maxPathWidth += 2;

    // Tạo các hàng dữ liệu cho bảng
    std::vector<Element> table_rows;

    // Thêm tiêu đề bảng
    table_rows.push_back(
        hbox({
            text("ID") | bold | center | size(WIDTH, EQUAL, maxIdWidth),
            text("Song Name") | bold | center | size(WIDTH, EQUAL, maxNameWidth),
            text("File Path") | bold | center | size(WIDTH, EQUAL, maxPathWidth)
        }) | border
    );

    // Thêm các hàng dữ liệu
    for (size_t i = 0; i < songs.size(); ++i) {
        table_rows.push_back(
            hbox({
                text(std::to_string(i + 1)) | center | size(WIDTH, EQUAL, maxIdWidth),
                text(songs[i].getName()) | size(WIDTH, EQUAL, maxNameWidth),
                text(songs[i].getPath()) | size(WIDTH, EQUAL, maxPathWidth)
            })
        );
    }

    // Tạo bảng
    auto table = vbox(std::move(table_rows));

    // Tạo renderer cho chi tiết playlist
    auto renderer = Renderer([&] {
        return vbox({
            text("=== Playlist Details: " + playlist.getName() + " ===") | bold | center,
            separator(),
            table | border | center,  // Hiển thị bảng với border
            separator(),
            text("Press ENTER to return.") | dim | center
        }) | center;
    });

    // Xử lý sự kiện nhấn ENTER
    auto screen = ScreenInteractive::TerminalOutput();
    auto main_component = CatchEvent(renderer, [&](Event event) {
        if (event == Event::Return) {
            screen.ExitLoopClosure()();
            return true;
        }
        return false;
    });

    // Hiển thị màn hình
    screen.Loop(main_component);
}


