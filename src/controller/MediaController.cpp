#include "./controller/MediaController.h"
#include <iostream>

MediaController::MediaController() {}

void MediaController::addMediaFile(const std::string &path, const std::string &type)
{
    mediaFiles.push_back(MediaFile(path, type));
}

void MediaController::listMediaFiles()
{
    std::string mediaList;
    for (const auto &file : mediaFiles)
    {
        mediaList += file.getName() + "\n";
    }
    view.showMediaList(mediaList);
}

void MediaController::playMediaFile(const MediaFile &media)
{
    // Mock playback information
    std::string playbackInfo = "Playing: " + media.getName();
    view.showPlaybackInfo(playbackInfo);
}

void MediaController::showMetadata(const MediaFile &media)
{
    view.showMetadata(media.getMetadata());
}

void MediaController::editMetadata(MediaFile &media, const std::string &key, const std::string &value)
{
    media.editMetadata(key, value);
    showMetadata(media);
}
