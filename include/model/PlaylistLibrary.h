#ifndef PLAYLIST_LIBRARY_H
#define PLAYLIST_LIBRARY_H

#include <vector>
#include <string>
#include "model/Playlist.h" // Thêm include để định nghĩa lớp Playlist

class PlaylistLibrary {
private:
    std::vector<Playlist> playlists; // Danh sách playlist

public:
    // Lấy danh sách tất cả các playlist
    std::vector<Playlist> &getPlaylists() ;

    // Thêm một playlist mới vào thư viện
    void addPlaylist(const Playlist& playlist);

    // Xóa playlist theo tên
    void removePlaylist(const std::string& name);

    // Lấy playlist theo tên
    Playlist* getPlaylistByName(const std::string& name);

    // Lưu tất cả playlist vào file
    void saveToFile(const std::string& filename) const;

    // Tải các playlist từ file
    void loadFromFile(const std::string& filename);
};

#endif // PLAYLIST_LIBRARY_H
