#include "view/MusicPlayerView.h"
#include <iostream>

void MusicPlayerView::showSongInfo(const std::string& title, const std::string& artist, const std::string& album) {
    std::cout << "Song Information:\n";
    std::cout << "Title: " << title << "\n"
              << "Artist: " << artist << "\n"
              << "Album: " << album << std::endl;
}
