#include <iostream>
#include "common/TagManager.h"
#include "model/Song.h"
#include "controller/MusicManager.h"
#include "view/MusicPlayerView.h"

int main() {
    // Đọc thông tin từ file nhạc
    std::string filePath = "media/APT.mp3"; // Ví dụ đường dẫn đến file nhạc

    TagManager tagManager(filePath);
    Song song(tagManager.getTitle(), tagManager.getArtist(), tagManager.getAlbum());

    // Tạo đối tượng quản lý nhạc và view
    MusicManager musicManager;
    MusicPlayerView musicPlayerView;

    // Thêm bài hát vào quản lý
    musicManager.addSong(song);

    // Hiển thị thông tin bài hát
    musicPlayerView.showSongInfo(song.getTitle(), song.getArtist(), song.getAlbum());

    // Giả sử chúng ta chơi bài hát đầu tiên trong danh sách
    musicManager.playSong(0);

    return 0;
}
