#include "view/MetadataView.h"
#include "common/Exception.h"
#include <iostream>
#include <climits>

// void MetadataView::showMenu() {
//     cout << "\n+---------------------------------+" << endl;
//     cout << "|       Edit Metadata Menu        |" << endl;
//     cout << "+---------------------------------+" << endl;
//     cout << "| 1. Edit Title                   |" << endl;
//     cout << "| 2. Edit Artist                  |" << endl;
//     cout << "| 3. Edit Album                   |" << endl;
//     cout << "| 4. Edit Genre                   |" << endl;
//     cout << "| 5. Edit Year                    |" << endl;
//     cout << "| 6. Edit Track                   |" << endl;
//     cout << "| 0. Exit Editing                 |" << endl;
//     cout << "+---------------------------------+" << endl;
//     cout << "Enter your choice: ";
// }



// int MetadataView::handleInput() {
//     int choice = Exception::getValidatedInput_Int("Choose an option: ", Exception::validateInputMetadataView);
//     return choice;
// }

int MetadataView::showMenu() {
        // Danh sách hiển thị trên menu
        std::vector<std::string> menu_entries = {
            "1. Edit Title",    // Option 1
            "2. Edit Artist",   // Option 2
            "3. Edit Album",    // Option 3
            "4. Edit Genre",    // Option 4
            "5. Edit Year",     // Option 5
            "6. Edit Track",    // Option 6
            "0. Exit Editing"   // Option 0
        };

        // Ánh xạ từ chỉ số menu (0-based index) sang giá trị logic
        std::vector<int> logic_mapping = {1, 2, 3, 4, 5, 6, 0};  // Mảng ánh xạ logic

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
                text("==== Edit Metadata Menu ====") | center,
                separator(),
                menu->Render() | border,
                separator(),
                text("Use UP/DOWN keys, numbers (0-6), or click to navigate. Press ENTER to select.") | dim | center,
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

            // Khi nhấn phím số (0-6)
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


// void MetadataView::displayMetadata(const map<string, string>& metadata) {
//     // Hiển thị metadata
//     cout << "+---------------------+---------------------------+" << endl;
//     cout << "|       Field         |          Value           |" << endl;
//     cout << "+---------------------+---------------------------+" << endl;

//     for (const auto& field : metadata) {
//         cout << "| " << setw(20) << left << field.first
//                   << "| " << setw(25) << field.second << " |" << endl;
//     }
//     cout << "+---------------------+---------------------------+" << endl;
// }


void MetadataView::displayMetadata(const map<string, string>& metadata) {
    const int fieldWidth = 20;   // Chiều rộng của cột "Field"
    const int valueWidth = 40;  // Chiều rộng của cột "Value"
    const int totalWidth = fieldWidth + valueWidth + 5; // Tổng chiều rộng: cột Field + cột Value + khoảng cách

    // Hàm vẽ dòng ngang
    auto drawLine = [&]() {
        cout << "+" << string(fieldWidth + 1, '-') 
                  << "+" << string(valueWidth + 1, '-') 
                  << "+" << endl;
    };

    // Hiển thị header bảng
    drawLine();
    cout << "| " << setw(fieldWidth) << left << "Field"
              << "| " << setw(valueWidth) << left << "Value"
              << " |" << endl;
    drawLine();

    // Hiển thị nội dung metadata
    for (const auto& field : metadata) {
        string key = field.first;
        string value = field.second;

        // Cắt chuỗi giá trị nếu quá dài
        while (value.length() > valueWidth) {
            // In dòng đầu tiên với key
            cout << "| " << setw(fieldWidth) << left << key
                      << "| " << setw(valueWidth) << left << value.substr(0, valueWidth)
                      << " |" << endl;

            // Loại bỏ phần đã in và thay key bằng chuỗi trống
            value = value.substr(valueWidth);
            key = ""; // Key chỉ in ở dòng đầu tiên
        }

        // In dòng cuối cùng hoặc chuỗi ngắn
        cout << "| " << setw(fieldWidth) << left << key
                  << "| " << setw(valueWidth) << left << value
                  << " |" << endl;
    }

    // Hiển thị footer bảng
    drawLine();
}
