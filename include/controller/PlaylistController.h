#ifndef PLAYLISTCONTROLLER_H
#define PLAYLISTCONTROLLER_H

#include <string>
#include "common/BaseController.h"

/* Manage playlist actions */
class PlaylistController : public BaseController {
public:
    PlaylistController();

    void handleAction(int action) override;
    void createPlaylist(const string& name);
    void deletePlaylist();
    void viewPlaylistDetails(const string& name);
    virtual void listAllPlaylists();
    void playPlaylist(const string& name);
};

#endif
