#include "view/PlaylistView.h"
#include "common/Exception.h"
#include <iostream>
#include <iomanip> // For setw



int PlaylistView::showMenu() {
        // Danh sách hiển thị trên menu
        std::vector<std::string> menu_entries = {
            "1. Create a new playlist",         // Option 1
            "2. Delete a playlist",            // Option 2
            "3. View a playlist's details",    // Option 3
            "4. List all playlists",           // Option 4
            "5. Play ListSongs",
            "0. Exit"                          // Option 0
        };

        // Ánh xạ từ chỉ số menu (0-based index) sang giá trị logic
        std::vector<int> logic_mapping = {1, 2, 3, 4, 5,0};  // Mảng ánh xạ logic

        int selected = 0; // Chỉ số được FTXUI sử dụng (0-based index)
        std::string error_message; // Lưu trữ thông báo lỗi
        int final_selected = -1; // Giá trị cuối cùng trả về (logic value)

        // Tạo menu
        auto menu = Menu(&menu_entries, &selected);

        // Tạo màn hình
        auto screen = ScreenInteractive::TerminalOutput();

        // Tạo giao diện chính
        auto main_component = Renderer(menu, [&] {
            return vbox({
                text("=== Playlist Menu ===") | center,
                separator(),
                menu->Render() | border,
                separator(),
                text("Use UP/DOWN keys, numbers (0-5), or click to navigate. Press ENTER to select.") | dim | center,
                separator(),
                text(error_message) | color(Color::Red) | center // Hiển thị thông báo lỗi (nếu có)
            });
        });

        // Xử lý sự kiện
        main_component = CatchEvent(main_component, [&](Event event) {
            // Khi nhấn Enter
            if (event == Event::Return) {
                final_selected = logic_mapping[selected]; // Lấy giá trị logic từ ánh xạ
                screen.ExitLoopClosure()(); // Thoát vòng lặp giao diện
                return true;
            }

            // Khi nhấn phím số (0-4)
            if (event.is_character() && std::isdigit(event.character()[0])) {
                int number = event.character()[0] - '0'; // Chuyển ký tự thành số
                auto it = std::find(logic_mapping.begin(), logic_mapping.end(), number);
                if (it != logic_mapping.end()) {
                    final_selected = number; // Gán giá trị logic
                    screen.ExitLoopClosure()(); // Thoát giao diện
                    return true;
                } else {
                    error_message = "Invalid input: number not in menu!"; // Hiển thị lỗi
                    return true;
                }
            }

            // Xử lý sự kiện click chuột
            if (event.is_mouse() && event.mouse().button == Mouse::Left && event.mouse().motion == Mouse::Pressed) {
                int clicked_index = event.mouse().y - 3; // Tính chỉ số mục được click
                if (clicked_index >= 0 && clicked_index < (int)menu_entries.size()) {
                    final_selected = logic_mapping[clicked_index]; // Lấy giá trị logic từ ánh xạ
                    screen.ExitLoopClosure()(); // Thoát giao diện
                    return true;
                } else {
                    error_message = "Invalid click: out of menu range!"; // Hiển thị lỗi
                    return true;
                }
            }

            // Khi di chuyển bằng phím mũi tên
            if (event == Event::ArrowUp || event == Event::ArrowDown) {
                menu->OnEvent(event); // Xử lý phím mũi tên để di chuyển trong menu
                return true;
            }

            return false; // Không xử lý sự kiện nào khác
        });

        // Hiển thị giao diện
        screen.Loop(main_component);

        return final_selected; // Trả về giá trị logic đã chọn
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
