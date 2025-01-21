#ifndef MANAGERMODEL_H
#define MANAGERMODEL_H

#include <memory>
#include "model/MediaLibrary.h"
#include "model/PlayingMediaModel.h"
#include "model/PlaylistLibrary.h"

/* Manage media and playlist data */
class ManagerModel {
private:
    unique_ptr<MediaLibrary> mediaLibrary;
    unique_ptr<PlaylistLibrary> playlistLibrary;
    unique_ptr<PlayingMediaModel> playingMediaModel;
    ManagerModel();
public:
    ManagerModel(const ManagerModel&) = delete;
    ManagerModel& operator=(const ManagerModel&) = delete;
    static ManagerModel& getInstance();
    MediaLibrary& getMediaLibrary();
    PlaylistLibrary& getPlaylistLibrary();
    PlayingMediaModel& getPlayingMedia();
};

#endif
