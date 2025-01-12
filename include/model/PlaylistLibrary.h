#ifndef PLAYLIST_LIBRARY_H
#define PLAYLIST_LIBRARY_H

#include <vector>
#include <memory>
#include "Playlist.h"

class PlaylistLibrary {
private:
    std::vector<std::unique_ptr<Playlist>> playlists;

public:
    void addPlaylist(const std::string& name);
    void removePlaylist(const std::string& name);
    Playlist* getPlaylist(const std::string& name);
    const std::vector<std::unique_ptr<Playlist>>& getAllPlaylists() const;
};

#endif // PLAYLIST_LIBRARY_H
