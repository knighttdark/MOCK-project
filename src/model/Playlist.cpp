#include "model/Playlist.h"
#include <algorithm>
#include <iostream>

// Constructor with name
Playlist::Playlist(const string& playlistName) {
    if (playlistName.empty()) {
        throw invalid_argument("Playlist name cannot be empty.");
    }
    name = playlistName;
}

// Get the playlist name
string Playlist::getName() const {
    return name;
}

// Get all songs in the playlist (const reference)
const vector<MediaFile>& Playlist::getSongs() const {
    return songs;
}

// Set the playlist name
void Playlist::setName(const string& newName) {
    if (newName.empty()) {
        throw invalid_argument("New playlist name cannot be empty.");
    }
    name = newName;
}

// Add a song to the playlist
void Playlist::addSong(const MediaFile& song) {
    if (!containsSong(song)) {
        songs.push_back(song);
    } else {
        cerr << "Warning: Song already exists in the playlist.\n";
    }
}

// Remove a song from the playlist
void Playlist::removeSong(const MediaFile& song) {
    auto it = remove_if(songs.begin(), songs.end(),
                             [&song](const MediaFile& s) {
                                 return s.getPath() == song.getPath();
                             });
    if (it != songs.end()) {
        songs.erase(it, songs.end());
    } else {
        cerr << "Warning: Song not found in the playlist.\n";
    }
}

// Check if the playlist contains a specific song
bool Playlist::containsSong(const MediaFile& song) const {
    return any_of(songs.begin(), songs.end(),
                       [&song](const MediaFile& s) {
                           return s.getPath() == song.getPath();
                       });
}

// Update a song in the playlist
bool Playlist::updateSong(const MediaFile& oldSong, const MediaFile& newSong) {
    auto it = find_if(songs.begin(), songs.end(),
                           [&oldSong](const MediaFile& s) {
                               return s.getPath() == oldSong.getPath();
                           });
    if (it != songs.end()) {
        *it = newSong;
        return true;
    }
    return false; // Song không tồn tại
}

// Get info about the playlist
string Playlist::getInfo() const {
    string info = "Playlist: " + name + "\n";
    if (songs.empty()) {
        info += "No songs in the playlist.\n";
    } else {
        info += "Songs (" + to_string(songs.size()) + "):\n";
        for (const auto& song : songs) {
            info += " - " + song.getName() + " (" + song.getPath() + ")\n";
        }
    }
    return info;
}
