#ifndef MANAGERMODEL_H
#define MANAGERMODEL_H

#include <memory>
#include "model/MediaLibrary.h"
#include "model/PlaylistLibrary.h"
//#include "PlaylistLibrary.hpp"
//#include "PlayingMedia.hpp"

using namespace std;

class ManagerModel {
private:
    // Smart pointers để tự động quản lý bộ nhớ
    unique_ptr<MediaLibrary> mediaLibrary;
    unique_ptr<PlaylistLibrary> playlistLibrary;
    //unique_ptr<PlayingMedia> playingMedia;

    // Private constructor (Singleton)
    ManagerModel();

public:
    // Xóa copy constructor và assignment operator
    ManagerModel(const ManagerModel&) = delete;
    ManagerModel& operator=(const ManagerModel&) = delete;

    // Static method để lấy instance duy nhất
    static ManagerModel& getInstance();

    // Getters
    MediaLibrary& getMediaLibrary();
    PlaylistLibrary& getPlaylistLibrary();
    //PlayingMedia& getPlayingMedia();
};

#endif // MANAGERMODEL_HPP
