#include "model/PlaylistLibrary.h"
#include "model/Playlist.h"
#include "model/MediaFile.h"
#include <fstream>
#include <stdexcept>
#include <sstream>
#include <algorithm>



vector<Playlist>& PlaylistLibrary::getPlaylists() {
    return playlists;
}




void PlaylistLibrary::addPlaylist(const Playlist& playlist) {
    playlists.push_back(playlist);
}


void PlaylistLibrary::removePlaylist(const string& name) {
    playlists.erase(remove_if(playlists.begin(), playlists.end(),
                                   [&name](const Playlist& p) {
                                       return p.getName() == name;
                                   }),
                    playlists.end());
}


Playlist* PlaylistLibrary::getPlaylistByName(const string& name) {
    for (auto& playlist : playlists) {
        if (playlist.getName() == name) {
            return &playlist;
        }
    }
    return nullptr;
}


void PlaylistLibrary::saveToFile(const string& filename) const {
    ofstream outFile(filename, ios::out | ios::trunc);
    if (!outFile.is_open()) {
        throw runtime_error("Unable to open file for saving playlists.");
    }

    for (const auto& playlist : playlists) {
        outFile << playlist.getName() << '\n';
        for (const auto& song : playlist.getSongs()) {
            outFile << song.getName() << ',' << song.getPath() << ',' << song.getType() << '\n';
        }
        outFile << "#END_PLAYLIST\n";
    }

    outFile.close();
}


void PlaylistLibrary::loadFromFile(const string& filename) {
    ifstream inFile(filename, ios::in);
    if (!inFile.is_open()) {
        throw runtime_error("Unable to open file for loading playlists.");
    }

    playlists.clear();
    string line;
    Playlist* currentPlaylist = nullptr;

    while (getline(inFile, line)) {
        if (line.empty()) {
            continue;
        }

        if (line == "#END_PLAYLIST") {
            currentPlaylist = nullptr;
        } else if (currentPlaylist == nullptr) {
            currentPlaylist = &playlists.emplace_back(line);
        } else {
            istringstream ss(line);
            string songName, songPath, songType;

            if (getline(ss, songName, ',') && getline(ss, songPath, ',') && getline(ss, songType)) {
                currentPlaylist->addSong(MediaFile(-1, songName, songPath, songType));
            }
        }
    }

    inFile.close();
}
