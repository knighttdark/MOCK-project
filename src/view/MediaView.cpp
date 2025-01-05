#include "./view/MediaView.h"
#include <iostream>

void MediaView::showMediaList(const std::string &mediaList)
{
    std::cout << "Media Files: \n"
              << mediaList << std::endl;
}

void MediaView::showMetadata(const std::string &metadata)
{
    std::cout << "Metadata: \n"
              << metadata << std::endl;
}

void MediaView::showPlaybackInfo(const std::string &playbackInfo)
{
    std::cout << "Playback Info: \n"
              << playbackInfo << std::endl;
}
