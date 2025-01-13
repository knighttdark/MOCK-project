#include "view/MediaFileView.h"
#include <common/TerminalUtils.h>
#include <common/Exception.h>
#include <iostream>
#include <climits>

void MediaFileView::showMenu() {
    cout << "\n==== Media File View ====" << endl;
    cout << "1. Show Metadata" << endl;
    cout << "2. Next page" << endl;
    cout << "3. Previous page" << endl;
    cout << "4. Play by ID" << endl;
    cout << "5. Add Song to Playlist" << endl;
    cout << "0. Return Home" << endl;
}

int MediaFileView::handleInput() {
    int choice = Exception::getValidatedInput_Int("Choose an option: ", Exception::validateInputMediaFileView);
    return choice;
}

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
    string path = Exception::getValidatedInput_FilePath("Enter directory path: ");
    // cout << "Enter directory path: ";
    // cin >> path;
    // cin.ignore(INT_MAX, '\n');
    return path;
}

void MediaFileView::showOptionScan() {
    clearTerminal();
    cout << "\n==== Scan Options ====\n";
    cout << "1. Scan Directory" << endl;
    cout << "2. Scan USB" << endl;
}

int MediaFileView::handleInputOptionScan() {
    int choice = Exception::getValidatedInput_Int("Choose an option: ", Exception::validateInputOptionScan);
    return choice;
}