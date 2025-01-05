#ifndef MEDIACONTROLLER_H
#define MEDIACONTROLLER_H

#include "view/MediaView.h"
#include "model/MediaFile.h"
#include <vector>
#include <string>

class MediaController
{
public:
    MediaController();
    void listMediaFiles();
    void addMediaFile(const std::string &path, const std::string &type);
    void playMediaFile(const MediaFile &media);
    void showMetadata(const MediaFile &media);
    void editMetadata(MediaFile &media, const std::string &key, const std::string &value);

private:
    MediaView view;
    std::vector<MediaFile> mediaFiles;
};

#endif
