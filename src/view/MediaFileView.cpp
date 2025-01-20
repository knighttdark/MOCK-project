#include "view/MediaFileView.h"
#include <common/TerminalUtils.h>
#include <common/Exception.h>
#include "controller/MediaFileController.h"
#include "common/Enum.h"

int MediaFileView::getSelectedMediaID() const {
    return selected_media_ID;
}

// Setter cho selected_media_ID
void MediaFileView::setSelectedMediaID(int id) {
    selected_media_ID = id;
}

int MediaFileView::showMenu() {
    MenuRenderer menu("Media File Menu", 
                      {"1. Show Metadata", 
                       "2. Play Song", "3. Add to Playlist","4. Return to Playing", "0. Return Home"},
                      {1, 2, 3, 4, 0});
    return menu.render();
}


// void MediaFileView::displayMediaFiles(const vector<string>& medialist, int page) {
//     cout << "\n==== Media Files (Page " << page << ") ====\n";
//     for (const auto& file : medialist) {
//         cout << file << '\n';
//     }
// }
void MediaFileView::displayMediaFiles(const vector<string>& medialist, int page, const string& notification_message) {
    if (medialist.empty()) {
        auto empty_renderer = Renderer([] {
            return vbox({
                text("No media files available.") | bold | center,
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

    // Biến theo dõi bài hát được chọn
    int selected_index = 0;

    // Tạo Menu từ danh sách bài hát
    auto media_menu = Menu(&medialist, &selected_index);

    // Tạo Màn hình FTXUI
    auto screen = ScreenInteractive::TerminalOutput();

    // Renderer cho danh sách bài hát
    auto main_renderer = Renderer(media_menu, [&] {
        return vbox({
                   text("==== Media Files (Page " + std::to_string(page) +") ====") | bold | center,
                   separator(),
                   media_menu->Render() | border, // Hiển thị danh sách bài hát
                   separator(),
                   text(notification_message) | color(Color::Green) | center,
                   separator(),
                   hbox({
                       text("Press ENTER to select | "),
                       text("Page Controls: <- Previous | -> Next ") | dim
                   }) | center
               }) |
               center;
    });

    // Biến flag cho điều hướng trang
    bool next_page = false;
    bool previous_page = false;
    bool item_selected = false;

    // Bắt sự kiện nhập từ người dùng
    auto main_component = CatchEvent(main_renderer, [&](Event event) {
        // Xử lý khi nhấn Enter
        if (event == Event::Return) {
            if (selected_index >= 0 && selected_index < (int)medialist.size()) {
                setSelectedMediaID(selected_index + 1);  // Lưu ID của bài hát được chọn
                screen.ExitLoopClosure()();             // Thoát khỏi giao diện
            }
            return true;
        }

        // Xử lý sự kiện Arrow Right (Next Page)
        if (event == Event::ArrowRight) {
            next_page = true;  // Flag cho trang kế tiếp
            screen.ExitLoopClosure()();
            return true;
        }

        // Xử lý sự kiện Arrow Left (Previous Page)
        if (event == Event::ArrowLeft) {
            previous_page = true;  // Flag cho trang trước đó
            screen.ExitLoopClosure()();
            return true;
        }

        // Xử lý sự kiện click chuột
        if (event.is_mouse() && event.mouse().button == Mouse::Left && event.mouse().motion == Mouse::Pressed) {
            int clicked_index = event.mouse().y - 3; // Điều chỉnh để khớp vị trí menu
            if (clicked_index >= 0 && clicked_index < (int)medialist.size()) {
                selected_index = clicked_index;
                setSelectedMediaID(selected_index + 1); // Lưu ID của bài hát được chọn
                screen.ExitLoopClosure()();            // Thoát khỏi giao diện
            }
            return true;
        }

        // Xử lý các sự kiện khác (mũi tên lên/xuống)
        return media_menu->OnEvent(event);
    });

    // Chạy giao diện
    screen.Loop(main_component);

    // Xử lý sau khi thoát giao diện
    if (next_page) {
        MediaFileController* mediaFileController = dynamic_cast<MediaFileController*>(
            ManagerController::getInstance().getController("MediaFile"));

        /* Check if the MediaFileController is available */
        if (!mediaFileController) {
            cerr << "Error: MediaFileController is not available!" << endl;
        }
        mediaFileController ->handleAction(ACTION_NEXT_PAGE);
        
    } else if (previous_page) {
        MediaFileController* mediaFileController = dynamic_cast<MediaFileController*>(
            ManagerController::getInstance().getController("MediaFile"));

        /* Check if the MediaFileController is available */
        if (!mediaFileController) {
            cerr << "Error: MediaFileController is not available!" << endl;
        }
        mediaFileController ->handleAction(ACTION_PREVIOUS_PAGE);
    }
}

void MediaFileView::displayPagination(int currentPage, int totalPages) {
    cout << "\nPage " << currentPage << " of " << totalPages << '\n';
}



string MediaFileView::promptDirectoryInput() {
    string path = "";
    string error_message = ""; // Lưu thông báo lỗi nếu có

    // Tạo một ô nhập liệu cho người dùng
    auto input_box = Input(&path, "Enter directory path:");

    // Tạo màn hình giao diện
    auto screen = ScreenInteractive::TerminalOutput();

    // Tạo giao diện chính
    auto main_component = Renderer(input_box, [&] {
        return vbox({
            text("==== Enter Directory Path ====") | center,
            separator(),
            input_box->Render() | border,
            separator(),
            error_message.empty()
                ? text("Press Enter to confirm the input.") | dim | center
                : text(error_message) | color(Color::Red) | center, // Hiển thị lỗi nếu có
        });
    });

    // Xử lý sự kiện trong giao diện
    main_component = CatchEvent(main_component, [&](Event event) {
        if (event == Event::Return) { // Khi nhấn Enter
            try {
                Exception::checkInputFilePath(path); // Kiểm tra tính hợp lệ của đường dẫn
                screen.ExitLoopClosure()(); // Thoát vòng lặp giao diện nếu hợp lệ
                return true;
            } catch (const invalid_argument& e) {
                error_message = e.what(); // Cập nhật thông báo lỗi
                return true; // Cập nhật lại giao diện
            }
        }

        return false; // Không xử lý các sự kiện khác
    });

    // Hiển thị giao diện và chờ người dùng nhập liệu
    screen.Loop(main_component);

    return path;
}



int MediaFileView::showOptionScan() {
        // Danh sách hiển thị trên menu
        std::vector<std::string> menu_entries = {
            "1. Scan Directory",  // Option 1
            "2. Scan USB",        // Option 2
            "0. Return Home"      // Option 0
        };

        // Ánh xạ từ chỉ số menu (0-based index) sang giá trị logic
        std::vector<int> logic_mapping = {1, 2, 0};  // Mảng ánh xạ logic (1 - Scan Directory, 2 - Scan USB, 0 - Return Home)

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
                text("==== Scan Options ====") | center,
                separator(),
                menu->Render() | border,
                separator(),
                text("Use UP/DOWN keys, numbers (0-2), or click to navigate. Press ENTER to select.") | dim | center,
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

            // Khi nhấn phím số (0-2)
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