#include "view/MediaView.h"
#include <iostream>

void MediaView::displayMetadata(const std::string& filepath) {
    std::cout << "Displaying metadata for: " << filepath << std::endl;
    // Hiển thị chi tiết metadata
    std::cout << "Title: " << filepath << std::endl;
    // Giả sử bạn đã thêm một đối tượng Metadata vào để hiển thị các thông tin này
    // metadata sẽ được truyền từ MediaController
}

void MediaView::showError(const std::string& message) {
    std::cerr << "Error: " << message << std::endl;
}
