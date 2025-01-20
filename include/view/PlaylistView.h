#ifndef PLAYLISTVIEW_H
#define PLAYLISTVIEW_H

#include "common/BaseView.h"
#include "model/Playlist.h"
#include <vector>
#include <string>
#include "common/MenuRenderer.h"


class PlaylistView : public BaseView {
public:
    
    
    int showMenu() override; 
    void displayPlaylists(const vector<Playlist>& playlists); 
    void displayPlaylistDetails(const Playlist& playlist); 
};

#endif 
