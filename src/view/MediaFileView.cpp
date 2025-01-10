#include "view/MediaFileView.h"
#include <iostream>

void MediaFileView::showMenu() {
    std::cout << "\n==== Media File View ====" << std::endl;
    std::cout << "1. Show Metadata" << std::endl;
    std::cout << "2. Edit Metadata" << std::endl;
    std::cout << "3. Next page" << std::endl;
    std::cout << "4. Previous page" << std::endl;
    std::cout << "5. Play by ID" << std::endl;
    std::cout << "6. Return Home" << std::endl;
}

int MediaFileView::handleInput() {
    int choice;
    std::cout << "\nEnter your choice: ";
    std::cin >> choice;
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
    return path;
}