#ifndef MEDIALIBRARY_H
#define MEDIALIBRARY_H

#include <vector>
#include "MediaFile.h"

using namespace std;

class MediaLibrary {
private:
    vector<MediaFile> mediaFiles;

public:
    // Methods
    vector<MediaFile> getAllMediaFiles() const;
    void addMediaFile(const MediaFile& file);
    void removeMediaFile(const MediaFile& file);
    MediaFile getMediaFileByName(const string& name) const;
    int getTotalPages(int pageSize) const;
    vector<MediaFile> getMediaFilesForPage(int page, int pageSize) const;
};

#endif // MEDIALIBRARY_HPP
