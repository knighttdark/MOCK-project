#ifndef SONG_H
#define SONG_H

#include <string>

class Song {
private:
    std::string title;
    std::string artist;
    std::string album;

public:
    // Constructor chỉ khai báo ở đây
    Song(const std::string& t, const std::string& a, const std::string& al);

    // Getter methods
    std::string getTitle() const { return title; }
    std::string getArtist() const { return artist; }
    std::string getAlbum() const { return album; }

    // Setter methods (nếu cần)
    void setTitle(const std::string& t) { title = t; }
    void setArtist(const std::string& a) { artist = a; }
    void setAlbum(const std::string& al) { album = al; }
    
    // Phương thức display
    void display() const;
};

#endif // SONG_H
