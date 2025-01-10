#ifndef MEDIA_FILE_CONTROLLER_H
#define MEDIA_FILE_CONTROLLER_H

#include "common/BaseController.h"
#include "view/ManagerView.h"
#include <iostream>
#include <filesystem>

class MediaFileController : public BaseController {
private:
    ManagerView* manager;
    std::string filePath;     // Variable to store the file path (added here)

public:
    MediaFileController(ManagerView* managerView);
    void handleAction(int action) override;

        // Static method to validate if a file path exists
    static bool isValidFilePath(const std::string& path);

    // Method to set the file path
    void setFilePath(const std::string& path);
};

#endif // MEDIA_FILE_CONTROLLER_HPP
