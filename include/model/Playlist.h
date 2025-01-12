#ifndef PLAYLIST_H
#define PLAYLIST_H

#include <string>
#include <vector>
#include "MediaFile.h"

class Playlist {
private:
    std::string name;
    std::vector<MediaFile> mediaFiles;

public:
    Playlist(const std::string& name);

    const std::string& getName() const;
    void setName(const std::string& newName);

    void addMediaFile(const MediaFile& mediaFile);
    void removeMediaFile(const std::string& mediaFileName);
    const std::vector<MediaFile>& getMediaFiles() const;
};

#endif // PLAYLIST_H
