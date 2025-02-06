#ifndef PLAYLIST_LIBRARY_H
#define PLAYLIST_LIBRARY_H

#include <vector>
#include <string>
#include "model/Playlist.h"

/* Manage a collection of playlists */
class PlaylistLibrary {
private:
    vector<Playlist> playlists;

public:
    virtual vector<Playlist>& getPlaylists();
    void addPlaylist(const Playlist& playlist);
    void removePlaylist(const string& name);
    Playlist* getPlaylistByName(const string& name);
    virtual void saveToFile(const string& filename) const;
    void loadFromFile(const string& filename);
};

#endif
