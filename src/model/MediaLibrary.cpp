#include "model/MediaLibrary.h"
#include <filesystem>
#include <algorithm>
#include <iostream>
namespace fs = std::filesystem;

void MediaLibrary::addMediaFile(const MediaFile& file) {
    mediaFiles.push_back(file);
}

int MediaLibrary::getTotalPages(int pageSize) const {
    return (mediaFiles.size() + pageSize - 1) / pageSize;
}

std::vector<MediaFile> MediaLibrary::getMediaFilesForPage(int page, int pageSize) const {
    int start = page * pageSize;
    int end = std::min(static_cast<int>(mediaFiles.size()), start + pageSize);
    return std::vector<MediaFile>(mediaFiles.begin() + start, mediaFiles.begin() + end);
}

void MediaLibrary::scanDirectory(const std::string& path) {
    try {
        // Clear the current list of media files
        mediaFiles.clear();

        int currentIndex = 1; // Chỉ số bắt đầu từ 1
        for (const auto& entry : fs::directory_iterator(path)) {
            if (entry.is_regular_file()) {
                mediaFiles.emplace_back(
                    currentIndex++,                        // Gán index
                    entry.path().filename().string(),      // Tên tệp
                    entry.path().string(),                 // Đường dẫn
                    "unknown"                              // Loại tệp (mặc định là unknown)
                );
            }
        }
        std::cout << "Directory scanned and indexed successfully.\n";
    } catch (const std::exception& e) {
        std::cerr << "Error scanning directory: " << e.what() << std::endl;
    }
}

std::vector<MediaFile>& MediaLibrary::getMediaFiles() {
    return mediaFiles; // Trả về tham chiếu tới vector `mediaFiles`
}



void MediaLibrary::scanUSBDevice() {
    // Giả lập: USB thường gắn ở /media/username/
    scanDirectory("/media/username/");
}
