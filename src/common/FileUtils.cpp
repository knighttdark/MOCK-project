#include "common/FileUtils.h"
#include <filesystem>
#include <iostream>

namespace fs = std::filesystem;

void showAllFiles(const std::string& directory) {
    std::cout << "\nFiles in directory " << directory << ":\n" << std::endl;
    int index = 1;
    for (const auto& entry : fs::directory_iterator(directory)) {
        if (fs::is_regular_file(entry)) {
            std::cout << index++ << ". " << entry.path().filename().string() << std::endl;
        }
    }
}
