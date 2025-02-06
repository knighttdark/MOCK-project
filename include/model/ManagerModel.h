#ifndef MANAGERMODEL_H
#define MANAGERMODEL_H

#include <memory>
#include "model/MediaLibrary.h"
#include "model/PlayingMediaModel.h"
#include "model/PlaylistLibrary.h"

/* Manage media and playlist data */
class ManagerModel {
//private:
protected:
    static ManagerModel* instance;
    unique_ptr<MediaLibrary> mediaLibrary;
    unique_ptr<PlaylistLibrary> playlistLibrary;
    unique_ptr<PlayingMediaModel> playingMediaModel;
    ManagerModel();
public:
    friend class ManagerModelTest;
    friend class MediaFileControllerTest;
    static void setInstance(ManagerModel* mockInstance);
    ManagerModel(const ManagerModel&) = delete;
    ManagerModel& operator=(const ManagerModel&) = delete;
    static ManagerModel& getInstance();
    virtual MediaLibrary& getMediaLibrary();
    virtual PlaylistLibrary& getPlaylistLibrary();
    virtual PlayingMediaModel& getPlayingMedia();
};

#endif
