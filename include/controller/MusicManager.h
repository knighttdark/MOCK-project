#ifndef MUSICMANAGER_H
#define MUSICMANAGER_H

#include "model/Song.h"
#include <vector>

class MusicManager {
public:
    void addSong(const Song& song);
    void playSong(int index);
    void displaySongInfo(int index);

private:
    std::vector<Song> songs;
};

#endif
