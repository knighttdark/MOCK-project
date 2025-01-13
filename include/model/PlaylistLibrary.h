#ifndef PLAYLIST_LIBRARY_H
#define PLAYLIST_LIBRARY_H

#include <vector>
#include <string>
#include "model/Playlist.h"

/* Manage a collection of playlists */
class PlaylistLibrary {
private:
    vector<Playlist> playlists; /* List of playlists */

public:
    vector<Playlist>& getPlaylists(); /* Get all playlists */
    void addPlaylist(const Playlist& playlist); /* Add a playlist */
    void removePlaylist(const string& name); /* Remove a playlist by name */
    Playlist* getPlaylistByName(const string& name); /* Get playlist by name */
    void saveToFile(const string& filename) const; /* Save playlists to file */
    void loadFromFile(const string& filename); /* Load playlists from file */
};

#endif /* PLAYLIST_LIBRARY_H */
