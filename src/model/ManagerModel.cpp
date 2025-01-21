#include "model/ManagerModel.h"

ManagerModel::ManagerModel()
    : mediaLibrary(make_unique<MediaLibrary>()),
    playlistLibrary(make_unique<PlaylistLibrary>()),
    playingMediaModel(make_unique<PlayingMediaModel>()) {
}

ManagerModel& ManagerModel::getInstance() {
    static ManagerModel instance;
    return instance;
}

MediaLibrary& ManagerModel::getMediaLibrary() {
    return *mediaLibrary;
}

PlaylistLibrary& ManagerModel::getPlaylistLibrary() {
   return *playlistLibrary;
}

PlayingMediaModel& ManagerModel::getPlayingMedia() {
   return *playingMediaModel;
}
