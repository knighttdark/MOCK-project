#ifndef PLAYLISTVIEW_H
#define PLAYLISTVIEW_H

#include "common/BaseView.h"
#include "model/Playlist.h"
#include <vector>
#include <string>
#include "common/MenuRenderer.h"


class PlaylistView : public BaseView {
private: 
    int selected_playlist_ID = -1;
    string selected_playlist_name;
    string selectedPath;
public:
    // void showMenu() override; /* Show menu */
    // int handleInput() override; /* Handle input */
        // Getter for selectedPath
    const string& getSelectedPath() const ;

    // Setter for selectedPath
    void setSelectedPath(const string& path);
    int showMenu() override; /* Show menu */
    void displayPlaylists(const vector<Playlist>& playlists); /* Display playlists */
    void displayPlaylistDetails(const Playlist& playlist); /* Display playlist details */

    int getSelectedPlaylistID() const;
    void setSelectedPlaylistID(int id);

    const string& getSelectedPlaylistName() const;
    void setSelectedPlaylistName( string name) ;

};

#endif 
