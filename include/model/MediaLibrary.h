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
    virtual int getTotalPages(int pageSize) const;
    virtual vector<MediaFile> getMediaFilesForPage(int page, int pageSize) const;
    virtual void scanDirectory(const string& path);
    void scanUSBDevice();
    virtual vector<MediaFile>& getMediaFiles();
};

#endif
