#ifndef PLAYLISTVIEW_H
#define PLAYLISTVIEW_H

#include "common/BaseView.h"
#include "model/Playlist.h"
#include <vector>
#include <string>

class PlaylistView : public BaseView {
public:
    void showMenu() override;
    int handleInput() override;

    // Updated methods
    void displayPlaylists(const std::vector<Playlist>& playlists);
    void displayPlaylistDetails(const Playlist& playlist);
};

#endif // PLAYLISTVIEW_H
