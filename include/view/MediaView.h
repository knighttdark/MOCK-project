#ifndef MEDIAVIEW_H
#define MEDIAVIEW_H

#include <string>

class MediaView
{
public:
    void showMediaList(const std::string &mediaList);
    void showMetadata(const std::string &metadata);
    void showPlaybackInfo(const std::string &playbackInfo);
};

#endif
