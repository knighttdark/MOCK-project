#include "view/MetadataView.h"
#include "common/exception.h"
#include <iostream>

void MetadataView::showMenu() {
    cout << "Metadata View Menu" << endl;
}

// int MetadataView::handleInput() {
//     int choice;
//     cin >> choice;
//     return choice;
// }

int MetadataView::handleInput() {
    int choice;
    while (true) {
        try {
            cout << "\nEnter your choice: ";
            cin >> choice;

            if (cin.fail()) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                throw InputException("Invalid input. Please enter a valid number.");
            }

            return choice;
        } catch (const InputException& e) {
            cerr << "Error: " << e.what() << endl;
        }
    }
}

void MetadataView::displayMetadata(const map<string, string>& metadata) {
    cout << "Metadata Information:" << endl;
    for (const auto& data : metadata) {
        cout << data.first << ": " << data.second << endl;
    }
}
