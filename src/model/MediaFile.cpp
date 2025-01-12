#include "model/MediaFile.h"
#include <iostream>

MediaFile::MediaFile(const std::string& filePath, const Metadata& metadata)
    : filePath(filePath), metadata(metadata) {}

MediaFile::MediaFile(int id, const std::string& name, const std::string& artist, const char* genre)
    : filePath("media/" + std::to_string(id) + "_" + name),
      metadata(name, artist, "", genre, 0, 0) {}

const std::string& MediaFile::getFilePath() const { return filePath; }

const Metadata& MediaFile::getMetadata() const { return metadata; }

std::string MediaFile::getName() const {
    size_t pos = filePath.find_last_of("/\\");
    return (pos == std::string::npos) ? filePath : filePath.substr(pos + 1);
}

void MediaFile::displayInfo() const {
    std::cout << "File Path: " << filePath << "\n";
    metadata.displayMetadata();
}
