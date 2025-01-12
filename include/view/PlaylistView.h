#ifndef PLAYLIST_VIEW_H
#define PLAYLIST_VIEW_H

#include <iostream>
#include <string>
#include "common/BaseView.h"
#include "model/Playlist.h"

class PlaylistView : public BaseView {
public:
    void showMenu() override;
    int handleInput() override;

    void displayPlaylistDetails(const Playlist& playlist);
};

#endif // PLAYLIST_VIEW_H
