#include "view/MetadataView.h"
#include <iostream>

void MetadataView::showMenu() {
    cout << "Metadata View Menu" << endl;
}

int MetadataView::handleInput() {
    int choice;
    cin >> choice;
    return choice;
}

void MetadataView::displayMetadata(const map<string, string>& metadata) {
    cout << "Metadata Information:" << endl;
    for (const auto& data : metadata) {
        cout << data.first << ": " << data.second << endl;
    }
}
