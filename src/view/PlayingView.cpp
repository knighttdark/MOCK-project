#include "view/PlayingView.h"
#include <iostream>
#include <iomanip>
#include "common/TerminalUtils.h"



// void PlayingView::showMenu() {
//     std::cout << "\n=== Playing Menu ===\n";
//     std::cout << "1. Play/Pause\n";
//     std::cout << "2. Next\n";
//     std::cout << "3. Previous\n";
//     std::cout << "4. Adjust Volume\n";
//     std::cout << "0. Exit\n";
//     std::cout << "Enter your choice: ";
//     std::cout.flush();
// }

// int PlayingView::handleInput() {
//     int choice;
//     std::cin >> choice;
//     return choice;
// }

int PlayingView::showMenu() {
        // Danh sách hiển thị trên menu
        std::vector<std::string> menu_entries = {
            "1. Play/Pause",     // Option 1
            "2. Next",           // Option 2
            "3. Previous",       // Option 3
            "4. Adjust Volume",  // Option 4
            "0. Exit"            // Option 0
        };

        // Ánh xạ từ chỉ số menu (0-based index) sang giá trị logic
        std::vector<int> logic_mapping = {1, 2, 3, 4, 0};  // Mảng ánh xạ logic

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
                text("==== Playing Menu ====") | center,
                separator(),
                menu->Render() | border,
                separator(),
                text("Use UP/DOWN keys, numbers (0-4), or click to navigate. Press ENTER to select.") | dim | center,
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

    void PlayingView::displayPlayingView(const std::string& currentMedia, int duration, int volume, int currentTime) {
        clearTerminal();
        std::cout << "\n=========================================\n";
        std::cout << "              NOW PLAYING                \n";
        std::cout << "=========================================\n";
        std::cout << "Media: " << (!currentMedia.empty() ? currentMedia : "Unknown") << "\n";

        if (duration > 0) {
            int minutes = duration / 60;
            int seconds = duration % 60;
            std::cout << "Duration: " << minutes << " min " << seconds << " sec\n";
        } else {
            std::cout << "Duration: Unknown\n";
        }

        std::cout << "Volume: " << volume << "%\n";

        // Hiển thị thanh tiến trình
        updateProgressBar(currentTime, duration);
        
        //Hiển thị menu thao tác
        std::cout << "\n\n=== Playing Menu ===\n";
        std::cout << "1. Play/Pause\n";
        std::cout << "2. Next\n";
        std::cout << "3. Previous\n";
        std::cout << "4. Adjust Volume\n";
        std::cout << "0. Exit\n";
        std::cout << "Enter your choice: ";
        std::cout.flush();
    }



void PlayingView::updateProgressBar(int currentTime, int duration) {
    int progressWidth = 30; // Chiều rộng của thanh tiến trình
    int progress = (duration > 0) ? (currentTime * progressWidth / duration) : 0;

    int currentMinutes = currentTime / 60;
    int currentSeconds = currentTime % 60;
    int totalMinutes = duration / 60;
    int totalSeconds = duration % 60;

    std::cout << "\rProgress: [";
    for (int i = 0; i < progressWidth; ++i) {
        if (i < progress) {
            std::cout << "#";
        } else {
            std::cout << "-";
        }
    }
    std::cout << "] " << std::setfill('0') << std::setw(2) << currentMinutes << ":"
              << std::setfill('0') << std::setw(2) << currentSeconds << " / "
              << std::setfill('0') << std::setw(2) << totalMinutes << ":"
              << std::setfill('0') << std::setw(2) << totalSeconds << "   ";
    std::cout.flush();
}

void PlayingView::clearView() {
    system("clear"); // For Linux/MacOS
    // system("cls"); // For Windows
}