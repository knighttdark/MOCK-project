#include "common/MenuRenderer.h"
#include <algorithm>
#include <iostream>
#include <cctype>

using namespace ftxui;

MenuRenderer::MenuRenderer(const std::string& title, const std::vector<std::string>& menu_entries, const std::vector<int>& logic_mapping)
    : title(title), menu_entries(menu_entries), logic_mapping(logic_mapping) {}

int MenuRenderer::render() {
    int selected = 0;               // Chỉ số mục menu được chọn (0-based index)
    std::string error_message;      // Thông báo lỗi
    int final_selected = -1;        // Giá trị logic cuối cùng được trả về

    // Tạo menu từ FTXUI
    auto menu = Menu(&menu_entries, &selected);
    auto screen = ScreenInteractive::TerminalOutput();

    // Renderer chính của menu
    auto main_component = Renderer(menu, [&] {
        return vbox({
            text(title) | center,                        // Hiển thị tiêu đề menu
            separator(),                                 // Dòng ngăn cách
            menu->Render() | border,                    // Hiển thị menu với border
            separator(),
            text("Use UP/DOWN keys, numbers, or click to navigate. Press ENTER to select.") | dim | center,
            separator(),
            text(error_message) | color(Color::Red) | center // Hiển thị lỗi (nếu có)
        });
    });

    // Bắt và xử lý các sự kiện nhập
    main_component = CatchEvent(main_component, [&](Event event) {
        // Xử lý khi nhấn Enter
        if (event == Event::Return) {
            final_selected = logic_mapping[selected];  // Gán giá trị logic từ mục đang chọn
            screen.ExitLoopClosure()();               // Thoát khỏi giao diện menu
            return true;
        }

        // Xử lý khi nhấn phím số (0-9)
        if (event.is_character() && std::isdigit(event.character()[0])) {
            int number = event.character()[0] - '0';  // Chuyển ký tự thành số
            auto it = std::find(logic_mapping.begin(), logic_mapping.end(), number);
            if (it != logic_mapping.end()) {
                final_selected = number;             // Gán giá trị logic nếu hợp lệ
                screen.ExitLoopClosure()();          // Thoát khỏi giao diện
                return true;
            } else {
                error_message = "Invalid input: number not in menu!"; // Thông báo lỗi
                return true;
            }
        }

        // Xử lý sự kiện điều hướng bằng phím mũi tên
        if (event == Event::ArrowUp || event == Event::ArrowDown) {
            menu->OnEvent(event);                   // FTXUI xử lý điều hướng
            return true;
        }

        // Xử lý click chuột
        if (event.is_mouse() && event.mouse().button == Mouse::Left && event.mouse().motion == Mouse::Pressed) {
            int clicked_index = event.mouse().y - 3;  // Chỉ số mục được click (tính từ vị trí chuột)
            if (clicked_index >= 0 && clicked_index < (int)menu_entries.size()) {
                final_selected = logic_mapping[clicked_index]; // Gán giá trị logic từ ánh xạ
                screen.ExitLoopClosure()();                   // Thoát khỏi giao diện
                return true;
            } else {
                error_message = "Invalid click: out of menu range!"; // Thông báo lỗi
                return true;
            }
        }

        return false; // Không xử lý sự kiện khác
    });

    // Hiển thị giao diện menu
    screen.Loop(main_component);

    // Trả về giá trị logic cuối cùng
    return final_selected;
}
