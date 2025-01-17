#ifndef PLAYLISTVIEW_H
#define PLAYLISTVIEW_H

#include "common/BaseView.h"
#include "model/Playlist.h"
#include <vector>
#include <string>

/* View for playlist operations */
class PlaylistView : public BaseView {
public:
    // void showMenu() override; /* Show menu */
    // int handleInput() override; /* Handle input */
    int showMenu() override; /* Show menu */
    void displayPlaylists(const vector<Playlist>& playlists); /* Display playlists */
    void displayPlaylistDetails(const Playlist& playlist); /* Display playlist details */
};

#endif /* PLAYLISTVIEW_H */