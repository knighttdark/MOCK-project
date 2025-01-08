#include "controller/MusicManager.h"
#include <iostream>

void MusicManager::addSong(const Song& song) {
    songs.push_back(song);
}

void MusicManager::playSong(int index) {
    if (index >= 0 && index < songs.size()) {
        std::cout << "Playing: " << songs[index].getTitle() << std::endl;
    }
}

void MusicManager::displaySongInfo(int index) {
    if (index >= 0 && index < songs.size()) {
        songs[index].display();
    }
}
