#include "view/MediaFileView.h"
#include <iostream>
#include <climits>
#include <common/TerminalUtils.h>

void MediaFileView::showMenu() {
    std::cout << "\n==== Media File View ====" << std::endl;
    std::cout << "1. Show Metadata" << std::endl;
    std::cout << "2. Next page" << std::endl;
    std::cout << "3. Previous page" << std::endl;
    std::cout << "4. Play by ID" << std::endl;
    std::cout << "5. Add Song to Playlist" << std::endl;
    std::cout << "0. Return Home" << std::endl;
}

int MediaFileView::handleInput() {
    int choice;
    std::cout << "\nEnter your choice: ";
    std::cin >> choice;
    std::cin.ignore(INT_MAX, '\n');
    return choice;
}

void MediaFileView::displayMediaFiles(const std::vector<std::string>& medialist, int page) {
    std::cout << "\n==== Media Files (Page " << page << ") ====\n";
    for (const auto& file : medialist) {
        std::cout << file << '\n';
    }
}

void MediaFileView::displayPagination(int currentPage, int totalPages) {
    std::cout << "\nPage " << currentPage << " of " << totalPages << '\n';
}

std::string MediaFileView::promptDirectoryInput() {
    std::string path;
    std::cout << "Enter directory path: ";
    std::cin >> path;
    std::cin.ignore(INT_MAX, '\n');
    return path;
}

void MediaFileView::showOptionScan() {
    clearTerminal();
    std::cout << "\n==== Scan Options ====\n";
    std::cout << "1. Scan Directory" << std::endl;
    std::cout << "2. Scan USB" << std::endl;
}

int MediaFileView::handleInputOptionScan() {
    int choice;
    std::cout << "\nEnter your choice: ";
    std::cin >> choice;
    std::cin.ignore(INT_MAX, '\n');
    return choice;
}