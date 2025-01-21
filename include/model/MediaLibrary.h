#ifndef MEDIA_LIBRARY_H
#define MEDIA_LIBRARY_H

#include <vector>
#include <string>
#include "MediaFile.h"

/* Manage media files */
class MediaLibrary {
private:
    vector<MediaFile> mediaFiles;
    int currentPage = 0;
public:
    void addMediaFile(const MediaFile& file);
    int getTotalPages(int pageSize) const;
    vector<MediaFile> getMediaFilesForPage(int page, int pageSize) const;
    void scanDirectory(const string& path);
    void scanUSBDevice();
    vector<MediaFile>& getMediaFiles();
};

#endif
