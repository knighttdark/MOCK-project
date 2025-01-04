# MOCK Project - Media Files Browser/Player

## Mô tả dự án

Đây là một ứng dụng CLI quản lý và phát nhạc/video trên hệ điều hành Linux. Ứng dụng cho phép người dùng duyệt và phát các file media (audio/video) từ các thư mục hoặc từ các thiết bị ngoại vi như USB. Các tính năng bao gồm:

- **Duyệt các file media** trong thư mục và các thư mục con (hỗ trợ phân trang).
- **Danh sách phát (Playlist)**: Quản lý các playlist, cho phép người dùng tạo, cập nhật và xóa playlist.
- **Metadata của file**: Hiển thị và chỉnh sửa metadata của các file audio và video (sử dụng thư viện Taglib).
- **Phát nhạc và điều khiển**: Phát nhạc bằng thư viện SDL2, hỗ trợ Play/Pause, Next/Previous, điều khiển âm lượng, và hiển thị thời gian phát.
- **Kết nối với bo mạch S32K144** để điều khiển âm lượng và hiển thị thông tin bài hát đang phát.

## Các liên kết

- [Github Repository](https://github.com/thanhetebkdn/MOCK-project)
- [Notion Documentation](https://thanhdeptrai.notion.site/MOCK-project-170f363afe3980b6bc66f2a9bb8b923d?pvs=4)

## Yêu cầu phần mềm

- **C/C++ compiler**: GCC hoặc Clang.
- **SDL2 library**: Được sử dụng để phát nhạc.
- **Taglib library**: Được sử dụng để xử lý metadata của file media.
- **S32K144 board**: Cần thiết để kết nối và điều khiển các chức năng như âm lượng, phát nhạc, v.v.

## Cài đặt và sử dụng

### Cài đặt phụ thuộc

Trước khi chạy ứng dụng, bạn cần cài đặt các thư viện cần thiết như SDL2 và Taglib:

```bash
sudo apt-get install libsdl2-dev libtag1-dev
```
