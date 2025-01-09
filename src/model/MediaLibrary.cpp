#include "model/MediaLibrary.h"
#include <algorithm>
// Get all media files
vector<MediaFile> MediaLibrary::getAllMediaFiles() const {
    return mediaFiles;
}

// Add a media file
void MediaLibrary::addMediaFile(const MediaFile& file) {
    mediaFiles.push_back(file);
}

// Remove a media file
void MediaLibrary::removeMediaFile(const MediaFile& file) {
    mediaFiles.erase(remove(mediaFiles.begin(), mediaFiles.end(), file), mediaFiles.end());
}

// Get a media file by name
MediaFile MediaLibrary::getMediaFileByName(const string& name) const {
    for (const auto& file : mediaFiles) {
        if (file.getName() == name) {
            return file;
        }
    }
    return MediaFile();
}

// Get total pages
int MediaLibrary::getTotalPages(int pageSize) const {
    return (mediaFiles.size() + pageSize - 1) / pageSize;
}

// Get media files for a specific page
vector<MediaFile> MediaLibrary::getMediaFilesForPage(int page, int pageSize) const {
    vector<MediaFile> pageFiles;
    int start = page * pageSize;
    int end = min(static_cast<int>(mediaFiles.size()), start + pageSize);

    for (int i = start; i < end; ++i) {
        pageFiles.push_back(mediaFiles[i]);
    }

    return pageFiles;
}
