#include "model/ManagerModel.h"
#include "model/PlaylistLibrary.h"

// Private constructor
ManagerModel::ManagerModel()
    : mediaLibrary(make_unique<MediaLibrary>()),
    playlistLibrary(make_unique<PlaylistLibrary>())
    // playingMedia(make_unique<PlayingMedia>())
    {
}

// Static method để lấy instance duy nhất
ManagerModel& ManagerModel::getInstance() {
    static ManagerModel instance;
    return instance;
}

// Getters
MediaLibrary& ManagerModel::getMediaLibrary() {
    return *mediaLibrary;
}

PlaylistLibrary& ManagerModel::getPlaylistLibrary() {
   return *playlistLibrary;
}


//PlayingMedia& ManagerModel::getPlayingMedia() {
//    return *playingMedia;
//}
