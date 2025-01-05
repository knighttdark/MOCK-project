#include <iostream>
#include <string>
#include <filesystem>
#include "./controller/MediaController.h"

namespace fs = std::filesystem;

int main(int argc, char *argv[])
{
    std::string musicFolder = "media"; // Thư mục chứa nhạc

    // Kiểm tra thư mục media có tồn tại không
    if (!fs::exists(musicFolder) || !fs::is_directory(musicFolder))
    {
        std::cerr << "Invalid directory: " << musicFolder << std::endl;
        return 1;
    }

    MediaController controller;
    // Duyệt qua thư mục media và thêm các file media vào controller
    for (const auto &entry : fs::recursive_directory_iterator(musicFolder))
    {
        if (entry.is_regular_file())
        {
            std::string filePath = entry.path().string();
            std::string fileExtension = entry.path().extension().string();

            if (fileExtension == ".mp3" || fileExtension == ".wav" || fileExtension == ".mp4")
            {
                controller.addMediaFile(filePath, fileExtension);
            }
        }
    }

    controller.listMediaFiles();
    // Tiếp tục với các thao tác khác như phát nhạc, hiển thị metadata, v.v.

    return 0;
}
