#ifndef PLAYLISTVIEW_H
#define PLAYLISTVIEW_H

#include "common/BaseView.h"
#include "model/Playlist.h"
#include <vector>
#include <string>
#include "common/MenuRenderer.h"

/* View for playlist operations */
class PlaylistView : public BaseView {
private: 
    int selected_playlist_ID = -1;
    string selected_playlist_name;
public:
    // void showMenu() override; /* Show menu */
    // int handleInput() override; /* Handle input */
    int showMenu() override; /* Show menu */
    void displayPlaylists(const vector<Playlist>& playlists); /* Display playlists */
    void displayPlaylistDetails(const Playlist& playlist); /* Display playlist details */

    int getSelectedPlaylistID() const;
    void setSelectedPlaylistID(int id);

    const string& getSelectedPlaylistName() const;
    void setSelectedPlaylistName( string name) ;
};

#endif /* PLAYLISTVIEW_H */