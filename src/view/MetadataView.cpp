#include "view/MetadataView.h"
#include <iostream>
#include <climits>

void MetadataView::showMenu() {
    std::cout << "\n+---------------------------------+" << std::endl;
    std::cout << "|       Edit Metadata Menu        |" << std::endl;
    std::cout << "+---------------------------------+" << std::endl;
    std::cout << "| 1. Edit Title                   |" << std::endl;
    std::cout << "| 2. Edit Artist                  |" << std::endl;
    std::cout << "| 3. Edit Album                   |" << std::endl;
    std::cout << "| 4. Edit Genre                   |" << std::endl;
    std::cout << "| 5. Edit Year                    |" << std::endl;
    std::cout << "| 6. Edit Track                   |" << std::endl;
    std::cout << "| 7. Restore Original Values      |" << std::endl;
    std::cout << "| 0. Exit Editing                 |" << std::endl;
    std::cout << "+---------------------------------+" << std::endl;
    std::cout << "Enter your choice: ";
}



int MetadataView::handleInput() {
    int choice;
    std::cin >> choice;
    std::cin.ignore(INT_MAX, '\n');
    return choice;

}

// void MetadataView::displayMetadata(const std::map<std::string, std::string>& metadata) {
//     // Hiển thị metadata
//     std::cout << "+---------------------+---------------------------+" << std::endl;
//     std::cout << "|       Field         |          Value           |" << std::endl;
//     std::cout << "+---------------------+---------------------------+" << std::endl;

//     for (const auto& field : metadata) {
//         std::cout << "| " << std::setw(20) << std::left << field.first
//                   << "| " << std::setw(25) << field.second << " |" << std::endl;
//     }
//     std::cout << "+---------------------+---------------------------+" << std::endl;
// }


void MetadataView::displayMetadata(const std::map<std::string, std::string>& metadata) {
    const int fieldWidth = 20;   // Chiều rộng của cột "Field"
    const int valueWidth = 40;  // Chiều rộng của cột "Value"
    const int totalWidth = fieldWidth + valueWidth + 5; // Tổng chiều rộng: cột Field + cột Value + khoảng cách

    // Hàm vẽ dòng ngang
    auto drawLine = [&]() {
        std::cout << "+" << std::string(fieldWidth + 1, '-') 
                  << "+" << std::string(valueWidth + 1, '-') 
                  << "+" << std::endl;
    };

    // Hiển thị header bảng
    drawLine();
    std::cout << "| " << std::setw(fieldWidth) << std::left << "Field"
              << "| " << std::setw(valueWidth) << std::left << "Value"
              << " |" << std::endl;
    drawLine();

    // Hiển thị nội dung metadata
    for (const auto& field : metadata) {
        std::string key = field.first;
        std::string value = field.second;

        // Cắt chuỗi giá trị nếu quá dài
        while (value.length() > valueWidth) {
            // In dòng đầu tiên với key
            std::cout << "| " << std::setw(fieldWidth) << std::left << key
                      << "| " << std::setw(valueWidth) << std::left << value.substr(0, valueWidth)
                      << " |" << std::endl;

            // Loại bỏ phần đã in và thay key bằng chuỗi trống
            value = value.substr(valueWidth);
            key = ""; // Key chỉ in ở dòng đầu tiên
        }

        // In dòng cuối cùng hoặc chuỗi ngắn
        std::cout << "| " << std::setw(fieldWidth) << std::left << key
                  << "| " << std::setw(valueWidth) << std::left << value
                  << " |" << std::endl;
    }

    // Hiển thị footer bảng
    drawLine();
}
