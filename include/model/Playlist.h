#ifndef PLAYLIST_H
#define PLAYLIST_H

#include <string>
#include <vector>
#include "model/MediaFile.h"

/* Playlist class to manage a collection of media files */
class Playlist {
private:
    string name; /* Playlist name */
    vector<MediaFile> songs; /* List of songs */

public:
    Playlist() = default; /* Default constructor */
    explicit Playlist(const string& playlistName); /* Constructor with name */

    string getName() const; /* Get playlist name */
    const vector<MediaFile>& getSongs() const; /* Get songs */

    void setName(const string& newName); /* Set playlist name */
    void addSong(const MediaFile& song); /* Add a song */
    void removeSong(const MediaFile& song); /* Remove a song */
    bool containsSong(const MediaFile& song) const; /* Check if song exists */
    bool updateSong(const MediaFile& oldSong, const MediaFile& newSong); /* Update a song */

    string getInfo() const; /* Get playlist info */
};

#endif /* PLAYLIST_H */