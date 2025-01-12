#ifndef METADATA_H
#define METADATA_H

#include <string>
#include <iostream>

class Metadata {
private:
    std::string title;
    std::string artist;
    std::string album;
    std::string genre;
    int duration; // Thời lượng (giây)
    int year;

public:
    Metadata(const std::string& title = "", const std::string& artist = "",
             const std::string& album = "", const std::string& genre = "",
             int duration = 0, int year = 0);

    const std::string& getTitle() const;
    const std::string& getArtist() const;
    const std::string& getAlbum() const;
    const std::string& getGenre() const;
    int getDuration() const;
    int getYear() const;

    void displayMetadata() const;
};

#endif // METADATA_H
