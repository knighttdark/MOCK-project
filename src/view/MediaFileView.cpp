#include "view/MediaFileView.h"
#include "common/exception.h"
#include <iostream>

void MediaFileView::showMenu() {
    cout << "\n==== Media File View ====" << endl;
    cout << "1. Show Metadata" << endl;
    cout << "2. Edit Metadata" << endl;
    cout << "3. Next page" << endl;
    cout << "4. Previous page" << endl;
    cout << "5. Play by ID" << endl;
    cout << "6. Return Home" << endl;
}

int MediaFileView::handleInput() {
    int choice;
    while (true) {
        try {
            cout << "\nEnter your choice: ";
            cin >> choice;

            if (cin.fail() || choice < 1 || choice > 6) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                throw InputException("Invalid input. Please enter a number between 1 and 6.");
            }

            return choice;
        } catch (const InputException& e) {
            cerr << "Error: " << e.what() << endl;
        }
    }
}
// int MediaFileView::handleInput() {
//     int choice;
//     cout << "\nEnter your choice: ";
//     cin >> choice;
//     return choice;
// }

void MediaFileView::displayMediaFiles(const vector<string>& medialist, int page) {
    cout << "\n==== Media Files (Page " << page << ") ====\n";
    for (const auto& file : medialist) {
        cout << file << '\n';
    }
}

void MediaFileView::displayPagination(int currentPage, int totalPages) {
    cout << "\nPage " << currentPage << " of " << totalPages << '\n';
}

string MediaFileView::promptDirectoryInput() {
    string path;
    cout << "Enter directory path: ";
    cin >> path;
    return path;
}