#ifndef PLAYLIST_H
#define PLAYLIST_H

#include <string>
#include <vector>
#include "model/MediaFile.h"

/* Playlist class to manage a collection of media files */
class Playlist {
private:
    string name;
    vector<MediaFile> songs;
public:
    Playlist() = default;
    explicit Playlist(const string& playlistName);
    string getName() const;
    const vector<MediaFile>& getSongs() const;
    void setName(const string& newName);
    void addSong(const MediaFile& song);
    void removeSong(const MediaFile& song);
    bool containsSong(const MediaFile& song) const;
    bool updateSong(const MediaFile& oldSong, const MediaFile& newSong);
    string getInfo() const;
};

#endif
