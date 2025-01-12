#include "view/DefaultScreenView.h"
#include "view/ManagerView.h"
#include "common/exception.h"
#include <iostream>

void DefaultScreenView::showMenu() {
    cout << "=============================\n";
    cout << " Welcome to the Default Screen\n";
    cout << "=============================\n";
    cout << "1. Browse Media Files\n";
    cout << "2. Playlist\n";
    cout << "3. Now Playing\n";
    cout << "4. Exit\n";
}

// int DefaultScreenView::handleInput() {
//     int choice;
//     cout << "\nChoose an option: ";
//     cin >> choice;
//     return choice;
// }

int DefaultScreenView::handleInput() {
    int choice;
    while (true) {
        try {
            cout << "\nChoose an option: ";
            cin >> choice;

            if (cin.fail() || choice < 1 || choice > 4) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                throw InputException("Invalid input. Please enter a number between 1 and 4.");
            }

            return choice; // Trả về giá trị hợp lệ
        }
        catch (const InputException& e) {
            cerr << "Error: " << e.what() << endl; // In ra thông báo lỗi
        }
    }
}