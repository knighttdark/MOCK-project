#include "controller/MediaController.h"
#include "common/FileUtils.h"
#include "common/TagLibUtils.h"
#include "common/Metadata.h"
#include <iostream>
#include <filesystem>

namespace fs = std::filesystem;

void MediaController::showFileList(const std::string& directory) {
    std::vector<std::string> files;
    for (const auto& entry : fs::directory_iterator(directory)) {
        if (entry.is_regular_file()) {
            files.push_back(entry.path().filename().string());
        }
    }
    
    // Hiển thị danh sách các tệp
    for (size_t i = 0; i < files.size(); ++i) {
        std::cout << (i + 1) << ". " << files[i] << std::endl;
    }
}


void MediaController::editMetadata(const std::string& filepath) {
    if (!std::filesystem::exists(filepath)) {
        std::cerr << "Error: File does not exist: " << filepath << std::endl;
        return;
    }

    TagLib::FileRef f(filepath.c_str());

    if (!f.isNull() && f.tag() && f.audioProperties()) {
        TagLib::Tag* tag = f.tag();
        TagLib::AudioProperties* audioProperties = f.audioProperties();

        Metadata originalMetadata;
        originalMetadata.setValue("Title", tag->title().toCString());
        originalMetadata.setValue("Artist", tag->artist().toCString());
        originalMetadata.setValue("Album", tag->album().toCString());
        originalMetadata.setValue("Year", std::to_string(tag->year()));
        originalMetadata.setValue("Track", std::to_string(tag->track()));
        originalMetadata.setValue("Genre", tag->genre().toCString());

        printMetadata(tag, audioProperties);

        std::string newTitle;
        std::cout << "Enter new Title (leave blank to keep current): ";
        std::getline(std::cin, newTitle);
        if (!newTitle.empty()) {
            tag->setTitle(newTitle);
        }

        f.save();
        std::cout << "Metadata updated!" << std::endl;
    } else {
        std::cerr << "Error: Unable to read metadata for " << filepath << std::endl;
    }
}