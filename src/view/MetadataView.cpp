#include "view/MetadataView.h"
#include "common/Exception.h"
#include <iostream>
#include <climits>


int MetadataView::showMenu() {
    MenuRenderer menu("Edit Metadata Menu", 
                      {"1. Edit Title", "2. Edit Artist", "3. Edit Album", 
                       "4. Edit Genre", "5. Edit Year", "6. Edit Track", "0. Exit Editing"},
                      {1, 2, 3, 4, 5, 6, 0});
    return menu.render();
}




// void MetadataView::displayMetadata(const map<string, string>& metadata) {
//     const int fieldWidth = 20;   // Chiều rộng của cột "Field"
//     const int valueWidth = 40;  // Chiều rộng của cột "Value"
//     const int totalWidth = fieldWidth + valueWidth + 5; // Tổng chiều rộng: cột Field + cột Value + khoảng cách

//     // Hàm vẽ dòng ngang
//     auto drawLine = [&]() {
//         cout << "+" << string(fieldWidth + 1, '-') 
//                   << "+" << string(valueWidth + 1, '-') 
//                   << "+" << endl;
//     };

//     // Hiển thị header bảng
//     drawLine();
//     cout << "| " << "Field" 
//               << string(fieldWidth - 5, ' ') // Thêm khoảng trắng cho cột "Field"
//               << "| " << "Value" 
//               << string(valueWidth - 5, ' ') // Thêm khoảng trắng cho cột "Value"
//               << " |" << endl;
//     drawLine();

//     // Hiển thị nội dung metadata
//     for (const auto& field : metadata) {
//         string key = field.first;
//         string value = field.second;

//         // Cắt chuỗi giá trị nếu quá dài
//         while (value.length() > valueWidth) {
//             // In dòng đầu tiên với key
//             cout << "| " << key;
//             if (key.length() < fieldWidth) {
//                 cout << string(fieldWidth - key.length(), ' '); // Thêm khoảng trắng để căn chỉnh cột "Field"
//             }
//             cout << "| " << value.substr(0, valueWidth); // In một phần giá trị
//             if (valueWidth > value.substr(0, valueWidth).length()) {
//                 cout << string(valueWidth - value.substr(0, valueWidth).length(), ' '); // Thêm khoảng trắng
//             }
//             cout << " |\n";

//             // Loại bỏ phần đã in và thay key bằng chuỗi trống
//             value = value.substr(valueWidth);
//             key = ""; // Key chỉ in ở dòng đầu tiên
//         }

//         // In dòng cuối cùng hoặc chuỗi ngắn
//         cout << "| " << key;
//         if (key.length() < fieldWidth) {
//             cout << string(fieldWidth - key.length(), ' '); // Thêm khoảng trắng để căn chỉnh cột "Field"
//         }
//         cout << "| " << value;
//         if (value.length() < valueWidth) {
//             cout << string(valueWidth - value.length(), ' '); // Thêm khoảng trắng
//         }
//         cout << " |\n";
//     }

//     // Hiển thị footer bảng
//     drawLine();
// }

void MetadataView::displayMetadata(const map<string, string>& metadata) {
    // Tạo danh sách các hàng cho bảng
    std::vector<Element> table_rows;

    // Tiêu đề của bảng
    table_rows.push_back(
        hbox({
            text("Field") | bold | size(WIDTH, EQUAL, 20), // Cột Field rộng 20
            text("Value") | bold | size(WIDTH, EQUAL, 40)  // Cột Value rộng 40
        }) | border
    );

    // Thêm dữ liệu metadata vào bảng
    for (const auto& [key, value] : metadata) {
        std::string remaining_value = value;
        bool first_row = true;

        while (!remaining_value.empty()) {
            // Tách giá trị dài thành các dòng nhỏ
            std::string current_value = remaining_value.substr(0, 40);
            remaining_value = remaining_value.length() > 40 ? remaining_value.substr(40) : "";

            table_rows.push_back(
                hbox({
                    text(first_row ? key : "") | size(WIDTH, EQUAL, 20), // Chỉ hiển thị key ở dòng đầu
                    text(current_value) | size(WIDTH, EQUAL, 40)         // Hiển thị giá trị
                })
            );

            first_row = false; // Các dòng tiếp theo chỉ in giá trị, không in key
        }
    }

    // Tạo bảng
    auto table = vbox(std::move(table_rows)) | border;

    // Tạo renderer cho FTXUI
    auto renderer = Renderer([&] {
        return vbox({
            text("Metadata Viewer") | bold | center, // Tiêu đề chính
            separator(),
            table | center,                         // Bảng metadata
            separator(),
            text("Press ENTER to return.") | dim | center // Hướng dẫn
        }) | center;
    });

    // Hiển thị giao diện FTXUI
    auto screen = ScreenInteractive::TerminalOutput();
    auto main_component = CatchEvent(renderer, [&](Event event) {
        if (event == Event::Return) {
            screen.ExitLoopClosure()();
            return true;
        }
        return false;
    });

    screen.Loop(main_component);
}
