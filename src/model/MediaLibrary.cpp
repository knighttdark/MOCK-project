#include "model/MediaLibrary.h"
#include <filesystem>
#include <algorithm>
#include <iostream>
namespace fs = filesystem;

void MediaLibrary::addMediaFile(const MediaFile& file) {
    mediaFiles.push_back(file);
}

int MediaLibrary::getTotalPages(int pageSize) const {
    return (mediaFiles.size() + pageSize - 1) / pageSize;
}

vector<MediaFile> MediaLibrary::getMediaFilesForPage(int page, int pageSize) const {
    int start = page * pageSize;
    int end = min(static_cast<int>(mediaFiles.size()), start + pageSize);
    return vector<MediaFile>(mediaFiles.begin() + start, mediaFiles.begin() + end);
}

void MediaLibrary::scanDirectory(const string& path) {
    try {
        mediaFiles.clear();

        int currentIndex = 1;
        for (const auto& entry : fs::directory_iterator(path)) {
            if (entry.is_regular_file()) {
                string extension = entry.path().extension().string();

                if (extension == ".mp3" || extension == ".mp4") {
                    mediaFiles.emplace_back(
                        currentIndex++,
                        entry.path().filename().string(),
                        entry.path().string(),
                        extension
                    );
                }
            }
        }
    } catch (const exception& e) {
        cerr << "Error scanning directory: " << e.what() << endl;
    }
}

vector<MediaFile>& MediaLibrary::getMediaFiles() {
    return mediaFiles;
}



void MediaLibrary::scanUSBDevice() {
    scanDirectory("/media/username/");
}
