#include "view/MetadataView.h"
#include "common/Exception.h"
#include <iostream>
#include <climits>

void MetadataView::showMenu() {
    cout << "\n+---------------------------------+" << endl;
    cout << "|       Edit Metadata Menu        |" << endl;
    cout << "+---------------------------------+" << endl;
    cout << "| 1. Edit Title                   |" << endl;
    cout << "| 2. Edit Artist                  |" << endl;
    cout << "| 3. Edit Album                   |" << endl;
    cout << "| 4. Edit Genre                   |" << endl;
    cout << "| 5. Edit Year                    |" << endl;
    cout << "| 6. Edit Track                   |" << endl;
    cout << "| 0. Exit Editing                 |" << endl;
    cout << "+---------------------------------+" << endl;
    cout << "Enter your choice: ";
}



int MetadataView::handleInput() {
    int choice = Exception::getValidatedInput_Int("Choose an option: ", Exception::validateInputMetadataView);
    return choice;
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
    // const int totalWidth = fieldWidth + valueWidth + 5; // Tổng chiều rộng: cột Field + cột Value + khoảng cách

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
