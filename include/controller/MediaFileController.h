#ifndef MEDIA_FILE_CONTROLLER_H
#define MEDIA_FILE_CONTROLLER_H

#include "common/BaseController.h"
#include "model/MediaLibrary.h"
#include "view/MediaFileView.h"
#include "view/ManagerView.h"
#include "model/ManagerModel.h"
#include "controller/ManagerController.h"
#include <iostream>

// Define an enum for actions
enum MediaAction {
    SHOW_METADATA = 1,
    EDIT_METADATA,
    NEXT_PAGE,
    PREVIOUS_PAGE,
    PLAY_MEDIA,
    RETURN_HOME
};

class MediaFileController : public BaseController {
private:

    int currentPage = 0;              // Trang hiện tại
    const int pageSize = 25;          // Số tệp mỗi trang

public:
    MediaFileController();
    void scanDirectory(const string& path); // Quét thư mục
    void scanUSBDevice();                        // Quét thiết bị USB
    void nextPage();                             // Chuyển đến trang kế tiếp
    void previousPage();                         // Quay lại trang trước
    void handleAction(int action) override;      // Xử lý hành động từ người dùng
    void scanAndDisplayMedia();
};

#endif // MEDIA_FILE_CONTROLLER_H
