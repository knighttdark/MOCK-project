#ifndef MEDIA_LIBRARY_H
#define MEDIA_LIBRARY_H

#include <vector>
#include <string>
#include "MediaFile.h"

/* Manage media files */
class MediaLibrary {
private:
    vector<MediaFile> mediaFiles; /* List of media files */
    int currentPage = 0;          /* Current page number */

public:
    void addMediaFile(const MediaFile& file); /* Add a media file */
    int getTotalPages(int pageSize) const;    /* Get total pages */
    vector<MediaFile> getMediaFilesForPage(int page, int pageSize) const; /* Get media files for a page */
    void scanDirectory(const string& path);   /* Scan a directory */
    void scanUSBDevice();                     /* Scan USB devices */
    vector<MediaFile>& getMediaFiles();       /* Get all media files */
};

#endif /* MEDIA_LIBRARY_H */
