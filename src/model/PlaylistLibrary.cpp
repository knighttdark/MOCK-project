#include "model/PlaylistLibrary.h"
#include <algorithm>

void PlaylistLibrary::addPlaylist(const std::string& name) {
    playlists.push_back(std::make_unique<Playlist>(name));
}

void PlaylistLibrary::removePlaylist(const std::string& name) {
    playlists.erase(
        std::remove_if(playlists.begin(), playlists.end(),
                       [&name](const std::unique_ptr<Playlist>& playlist) {
                           return playlist->getName() == name;
                       }),
        playlists.end()
    );
}

Playlist* PlaylistLibrary::getPlaylist(const std::string& name) {
    for (auto& playlist : playlists) {
        if (playlist->getName() == name) {
            return playlist.get();
        }
    }
    return nullptr;
}

const std::vector<std::unique_ptr<Playlist>>& PlaylistLibrary::getAllPlaylists() const {
    return playlists;
}
