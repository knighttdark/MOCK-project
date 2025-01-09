#include "view/MetadataView.h"
#include <iostream>

void MetadataView::showMenu() {
    std::cout << "Metadata View Menu" << std::endl;
}

int MetadataView::handleInput() {
    int choice;
    std::cin >> choice;
    return choice;
}

void MetadataView::displayMetadata(const std::map<std::string, std::string>& metadata) {
    std::cout << "Metadata Information:" << std::endl;
    for (const auto& data : metadata) {
        std::cout << data.first << ": " << data.second << std::endl;
    }
}
