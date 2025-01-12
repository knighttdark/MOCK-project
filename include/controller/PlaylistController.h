#ifndef PLAYLIST_CONTROLLER_H
#define PLAYLIST_CONTROLLER_H

#include "common/BaseController.h"
#include "model/ManagerModel.h"
#include "view/PlaylistView.h"

class PlaylistController : public BaseController {
private:
    PlaylistView* playlistView;
    PlaylistLibrary& playlistLibrary;

public:
    PlaylistController();
    ~PlaylistController();

    void handleAction(int action) override;
};

#endif // PLAYLIST_CONTROLLER_H
