#ifndef PLAYLISTCONTROLLER_H
#define PLAYLISTCONTROLLER_H

#include <string>
#include "common/BaseController.h"

class PlaylistController : public BaseController {
public:
    PlaylistController();

    // Override handleAction
    void handleAction(int action) override;

    // Playlist-specific operations
    void createPlaylist(const std::string& name);
    void deletePlaylist();
    void viewPlaylistDetails(const std::string& name);
    void listAllPlaylists();
};

#endif // PLAYLISTCONTROLLER_H
