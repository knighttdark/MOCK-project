#include "model/Metadata.h"

Metadata::Metadata(const std::string& title, const std::string& artist,
                   const std::string& album, const std::string& genre,
                   int duration, int year)
    : title(title), artist(artist), album(album), genre(genre), duration(duration), year(year) {}

const std::string& Metadata::getTitle() const { return title; }
const std::string& Metadata::getArtist() const { return artist; }
const std::string& Metadata::getAlbum() const { return album; }
const std::string& Metadata::getGenre() const { return genre; }
int Metadata::getDuration() const { return duration; }
int Metadata::getYear() const { return year; }

void Metadata::displayMetadata() const {
    std::cout << "Title: " << title << "\n"
              << "Artist: " << artist << "\n"
              << "Album: " << album << "\n"
              << "Genre: " << genre << "\n"
              << "Duration: " << duration << " seconds\n"
              << "Year: " << year << "\n";
}
