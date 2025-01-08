#include "model/Song.h"
#include <iostream>

// Định nghĩa constructor trong .cpp
Song::Song(const std::string& title, const std::string& artist, const std::string& album)
    : title(title), artist(artist), album(album) {}

// Định nghĩa phương thức display
void Song::display() const {
    std::cout << "Title: " << title << "\n"
              << "Artist: " << artist << "\n"
              << "Album: " << album << std::endl;
}
