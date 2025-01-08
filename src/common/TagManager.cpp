#include "common/TagManager.h"
#include <iostream>

TagManager::TagManager(const std::string& filePath) {
    file = TagLib::FileRef(filePath.c_str());
    if (file.isNull()) {
        std::cerr << "Unable to open file." << std::endl;
    }
}

TagManager::~TagManager() {}

std::string TagManager::getTitle() const {
    return file.tag()->title().to8Bit();
}

std::string TagManager::getArtist() const {
    return file.tag()->artist().to8Bit();
}

std::string TagManager::getAlbum() const {
    return file.tag()->album().to8Bit();
}
