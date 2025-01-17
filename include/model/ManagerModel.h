#ifndef MANAGERMODEL_H
#define MANAGERMODEL_H

#include <memory>
#include "model/MediaLibrary.h"
// #include "model/PlayingMediaModel.h"
#include "model/PlaylistLibrary.h"

/* Manage media and playlist data */
class ManagerModel {
private:
    unique_ptr<MediaLibrary> mediaLibrary; /* Media library */
    unique_ptr<PlaylistLibrary> playlistLibrary; /* Playlist library */
    // unique_ptr<PlayingMediaModel> playingMediaModel; /* Playing media model */
    ManagerModel(); /* Private constructor */

public:
    ManagerModel(const ManagerModel&) = delete; /* No copy */
    ManagerModel& operator=(const ManagerModel&) = delete; /* No assign */

    static ManagerModel& getInstance(); /* Get instance */
    MediaLibrary& getMediaLibrary(); /* Get media library */
    PlaylistLibrary& getPlaylistLibrary(); /* Get playlist library */
    // PlayingMediaModel& getPlayingMedia(); /* Get playing media model */ 
};

#endif /* MANAGERMODEL_H */
