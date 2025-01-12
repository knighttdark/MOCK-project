#ifndef PLAYLISTVIEW_H
#define PLAYLISTVIEW_H

#include "common/BaseView.h"
#include <vector>
#include <string>

class PlaylistView : public BaseView {
public:
    void showMenu() override;
    int handleInput() override;
    void displayPlaylists(const vector<string>& playlists);
    void displayPlaylistDetails(const string& playlist);
};

#endif // PLAYLISTVIEW_HPP
