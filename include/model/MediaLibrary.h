#ifndef MEDIA_LIBRARY_H
#define MEDIA_LIBRARY_H

#include <vector>
#include <string>
#include "MediaFile.h"

class MediaLibrary {
private:
    vector<MediaFile> mediaFiles; // Danh sách tệp phương tiện
    int currentPage = 0;               // Trang hiện tại

public:
    void addMediaFile(const MediaFile& file); // Thêm tệp
    int getTotalPages(int pageSize) const;    // Tổng số trang
    vector<MediaFile> getMediaFilesForPage(int page, int pageSize) const; // Tệp theo trang
    void scanDirectory(const string& path); // Quét thư mục
    void scanUSBDevice();                        // Quét USB
};

#endif // MEDIA_LIBRARY_H