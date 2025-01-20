// MediaFile.cpp
#include "model/MediaFile.h"
#include <chrono>

// Default constructor
MediaFile::MediaFile() : index(-1) {}

// Parameterized constructor
MediaFile::MediaFile(int index, const string& name, const string& path, const string& type)
    : index(index), name(name), path(path), type(type) {}

// Getters and Setters
int MediaFile::getIndex() const {
    return index;
}

void MediaFile::setIndex(int newIndex) {
    index = newIndex;
}

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

void MediaFile::setMetadata(const Metadata& newMetadata) {
    metadata = newMetadata;
}

string MediaFile::getInfo() const {
    return name + " (Index: " + to_string(index) + ")";
}



int MediaFile::getDuration() const {

    return duration; 
}
