#include "model/Playlist.h"
#include <algorithm>

Playlist::Playlist(const std::string& name) : name(name) {}

const std::string& Playlist::getName() const { return name; }

void Playlist::setName(const std::string& newName) { name = newName; }

void Playlist::addMediaFile(const MediaFile& mediaFile) { mediaFiles.push_back(mediaFile); }

void Playlist::removeMediaFile(const std::string& mediaFileName) {
    mediaFiles.erase(
        std::remove_if(mediaFiles.begin(), mediaFiles.end(),
                       [&mediaFileName](const MediaFile& file) { return file.getName() == mediaFileName; }),
        mediaFiles.end());
}

const std::vector<MediaFile>& Playlist::getMediaFiles() const { return mediaFiles; }
