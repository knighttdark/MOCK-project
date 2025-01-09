#include "model/MediaFile.h"

// Constructor
MediaFile::MediaFile(const string& name, const string& path, const string& type)
    : name(name), path(path), type(type) {
}

// Getters
string MediaFile::getName() const {
    return name;
}

string MediaFile::getPath() const {
    return path;
}

string MediaFile::getType() const {
    return type;
}

Metadata MediaFile::getMetadata() const {
    return metadata;
}

// Setters
void MediaFile::setMetadata(const Metadata& metadata) {
    this->metadata = metadata;
}

bool MediaFile::operator==(const MediaFile& other) const {
    return name == other.name && path == other.path && type == other.type;
}
