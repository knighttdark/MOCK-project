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
    virtual void addPlaylist(const Playlist& playlist);
    virtual void removePlaylist(const string& name);
    virtual Playlist* getPlaylistByName(const string& name);
    virtual void saveToFile(const string& filename) const;
    void loadFromFile(const string& filename);
};

#endif
