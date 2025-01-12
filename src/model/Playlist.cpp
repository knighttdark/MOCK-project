#include "model/Playlist.h"
#include <algorithm>
#include <iostream>

// Constructor with name
Playlist::Playlist(const std::string& playlistName) {
    if (playlistName.empty()) {
        throw std::invalid_argument("Playlist name cannot be empty.");
    }
    name = playlistName;
}

// Get the playlist name
std::string Playlist::getName() const {
    return name;
}

// Get all songs in the playlist (const reference)
const std::vector<MediaFile>& Playlist::getSongs() const {
    return songs;
}

// Set the playlist name
void Playlist::setName(const std::string& newName) {
    if (newName.empty()) {
        throw std::invalid_argument("New playlist name cannot be empty.");
    }
    name = newName;
}

// Add a song to the playlist
void Playlist::addSong(const MediaFile& song) {
    if (!containsSong(song)) {
        songs.push_back(song);
    } else {
        std::cerr << "Warning: Song already exists in the playlist.\n";
    }
}

// Remove a song from the playlist
void Playlist::removeSong(const MediaFile& song) {
    auto it = std::remove_if(songs.begin(), songs.end(),
                             [&song](const MediaFile& s) {
                                 return s.getPath() == song.getPath();
                             });
    if (it != songs.end()) {
        songs.erase(it, songs.end());
    } else {
        std::cerr << "Warning: Song not found in the playlist.\n";
    }
}

// Check if the playlist contains a specific song
bool Playlist::containsSong(const MediaFile& song) const {
    return std::any_of(songs.begin(), songs.end(),
                       [&song](const MediaFile& s) {
                           return s.getPath() == song.getPath();
                       });
}

// Update a song in the playlist
bool Playlist::updateSong(const MediaFile& oldSong, const MediaFile& newSong) {
    auto it = std::find_if(songs.begin(), songs.end(),
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
std::string Playlist::getInfo() const {
    std::string info = "Playlist: " + name + "\n";
    if (songs.empty()) {
        info += "No songs in the playlist.\n";
    } else {
        info += "Songs (" + std::to_string(songs.size()) + "):\n";
        for (const auto& song : songs) {
            info += " - " + song.getName() + " (" + song.getPath() + ")\n";
        }
    }
    return info;
}
