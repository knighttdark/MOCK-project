#include "view/DefaultScreenView.h"
#include "view/ManagerView.h"
#include <common/TerminalUtils.h>
#include <common/Exception.h>
#include <iostream>
#include <climits>
// #include <ftxui/component/component.hpp>
// #include <ftxui/component/screen_interactive.hpp>

void DefaultScreenView::showMenu() {
    clearTerminal();
    cout << "=============================\n";
    cout << " Welcome to the Default Screen\n";
    cout << "=============================\n";
    cout << "1. Browse Media Files\n";
    cout << "2. Playlist\n";
    cout << "0. Exit\n";
}

int DefaultScreenView::handleInput() {
    int choice = Exception::getValidatedInput_Int("Choose an option: ", Exception::validateInputDefaultScreen);
    return choice;
}



// using namespace ftxui;

// void DefaultScreenView::showMenu() {
//     auto screen = ScreenInteractive::TerminalOutput();

//     // Danh sách menu
//     std::vector<std::string> menu_entries = {
//         "1. Browse Media Files",
//         "2. Playlist",
//         "0. Exit"
//     };

//     int selected = 0; // Chỉ số menu được chọn

//     // Menu component
//     auto menu = Menu(&menu_entries, &selected);

//     // Tạo giao diện
//     auto renderer = Renderer(menu, [&] {
//         return vbox({
//                    text("Welcome to the Default Screen") | bold | center,
//                    separator(),
//                    menu->Render() | border,
//                    separator(),
//                    text("Use Arrow keys, mouse, or numbers (1/2/0) to select.") | dim
//                }) |
//                center;
//     });

//     // Hiển thị giao diện
//     screen.Loop(renderer);
// }

// int DefaultScreenView::handleInput() {
//     auto screen = ScreenInteractive::TerminalOutput();

//     // Danh sách menu
//     std::vector<std::string> menu_entries = {
//         "1. Browse Media Files",
//         "2. Playlist",
//         "0. Exit"
//     };

//     int selected = 0; // Chỉ số menu được chọn
//     bool exit_menu = false; // Cờ thoát khỏi menu

//     // Menu component
//     auto menu = Menu(&menu_entries, &selected);

//     // Tạo giao diện
//     auto renderer = Renderer(menu, [&] {
//         return vbox({
//                    text("Welcome to the Default Screen") | bold | center,
//                    separator(),
//                    menu->Render() | border,
//                    separator(),
//                    text("Use Arrow keys, mouse, or numbers (1/2/0) to select.") | dim
//                }) |
//                center;
//     });

//     // Xử lý sự kiện
//     screen.Loop(renderer | CatchEvent([&](Event event) {
//         if (event.is_character()) { // Xử lý nhập số
//             std::string input_str = event.character();
//             if (!input_str.empty()) {
//                 char input = input_str[0]; // Lấy ký tự đầu tiên
//                 if (input == '0') { // Chọn "Exit"
//                     selected = 2; // Đặt thành Exit
//                     exit_menu = true;
//                 } else if (input == '1') { // Chọn "Browse Media Files"
//                     selected = 0;
//                     exit_menu = true;
//                 } else if (input == '2') { // Chọn "Playlist"
//                     selected = 1;
//                     exit_menu = true;
//                 }
//             }
//         }

//         // Nếu nhấn Enter
//         if (event == Event::Return) {
//             exit_menu = true;
//             return true;
//         }

//         // Xử lý nhấn chuột
//         if (event.is_mouse() && event.mouse().button == Mouse::Left && event.mouse().motion == Mouse::Pressed) {
//             int clicked_index = event.mouse().y - 3; // Adjust for header space
//             if (clicked_index >= 0 && clicked_index < (int)menu_entries.size()) {
//                 selected = clicked_index;
//                 exit_menu = true;
//             }
//         }

//         // Kết thúc khi có lựa chọn
//         if (exit_menu) {
//             screen.ExitLoopClosure()(); // Kết thúc giao diện
//         }

//         return false; // Bỏ qua các sự kiện khác
//     }));

//     // Sau khi ra khỏi vòng lặp, trả về giá trị tương ứng với lựa chọn:
//     return selected;
// }
