// MediaFile.cpp
#include "model/MediaFile.h"

// Default constructor
MediaFile::MediaFile() : index(-1) {}

// Parameterized constructor
MediaFile::MediaFile(int index, const std::string& name, const std::string& path, const std::string& type)
    : index(index), name(name), path(path), type(type) {}

// Getters and Setters
int MediaFile::getIndex() const {
    return index;
}

void MediaFile::setIndex(int newIndex) {
    index = newIndex;
}

std::string MediaFile::getName() const {
    return name;
}

std::string MediaFile::getPath() const {
    return path;
}

std::string MediaFile::getType() const {
    return type;
}

Metadata MediaFile::getMetadata() const {
    return metadata;
}

void MediaFile::setMetadata(const Metadata& newMetadata) {
    metadata = newMetadata;
}

std::string MediaFile::getInfo() const {
    return name + " (Index: " + std::to_string(index) + ")";
}
