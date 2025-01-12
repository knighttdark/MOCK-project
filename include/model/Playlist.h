#ifndef PLAYLIST_H
#define PLAYLIST_H

#include <string>
#include <vector>
#include "model/MediaFile.h"

class Playlist {
private:
    std::string name;                // Name of the playlist
    std::vector<MediaFile> songs;    // List of songs in the playlist

public:
    // Constructors
    Playlist() = default;
    explicit Playlist(const std::string& playlistName);

    // Getters
    std::string getName() const;
    const std::vector<MediaFile>& getSongs() const; // Return const reference

    // Setters
    void setName(const std::string& newName);

    // Playlist operations
    void addSong(const MediaFile& song);
    void removeSong(const MediaFile& song);
    bool containsSong(const MediaFile& song) const;

    // New operation: Update song
    bool updateSong(const MediaFile& oldSong, const MediaFile& newSong);

    // Utility
    std::string getInfo() const;
};

#endif // PLAYLIST_H
