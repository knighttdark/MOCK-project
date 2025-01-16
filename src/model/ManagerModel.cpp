#include "model/ManagerModel.h"

// Private constructor
ManagerModel::ManagerModel()
    : mediaLibrary(make_unique<MediaLibrary>()),
    playlistLibrary(make_unique<PlaylistLibrary>()),
    playingMediaModel(make_unique<PlayingMediaModel>()) {
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

PlayingMediaModel& ManagerModel::getPlayingMedia() {
   return *playingMediaModel;
}
