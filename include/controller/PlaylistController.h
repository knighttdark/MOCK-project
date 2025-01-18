#ifndef PLAYLISTCONTROLLER_H
#define PLAYLISTCONTROLLER_H

#include <string>
#include "common/BaseController.h"

/* Manage playlist actions */
class PlaylistController : public BaseController {
public:
    PlaylistController(); /* Constructor */

    void handleAction(int action) override; /* Handle user actions */
    void createPlaylist(const string& name); /* Create a playlist */
    void deletePlaylist(); /* Delete a playlist */
    void viewPlaylistDetails(const string& name); /* View playlist details */
    void listAllPlaylists(); /* List playlists */
    void playPlaylist(const string& name);
};

#endif /* PLAYLISTCONTROLLER_H */
