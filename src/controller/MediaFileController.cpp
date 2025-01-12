
#include "controller/MediaFileController.h"
#include <stdexcept>
#include <iostream>
#include <climits>
#include <common/TerminalUtils.h>

MediaFileController::MediaFileController(){}


void MediaFileController::scanDirectory(const std::string& path) {
    try {
        ManagerController::getInstance().getManagerModel()->getMediaLibrary().scanDirectory(path);
        std::cout << "Directory scanned successfully.\n";
    } catch (const std::exception& e) {
        std::cerr << "Error scanning directory: " << e.what() << std::endl;
    }
}


void MediaFileController::handleActionScan(int option) {
    MediaFileView* mediaFileView = dynamic_cast<MediaFileView*>(ManagerController::getInstance().getManagerView()->getView());
    if (!mediaFileView) {
        std::cerr << "Error: MediaFileView is null!" << std::endl;
        return;
    }

    switch (option) {
        case 1: { // Scan Directory
            std::string directoryPath = mediaFileView->promptDirectoryInput();
            scanDirectory(directoryPath);
            break;
        }
       case 2: { // Scan USB
            std::string usbRootPath = "/media/kist"; // Gốc thư mục chứa các USB
            std::vector<std::string> usbPaths;          // Danh sách đường dẫn USB

            // Liệt kê các thư mục con trong /media/username
            try {
                for (const auto& entry : std::filesystem::directory_iterator(usbRootPath)) {
                    if (entry.is_directory()) {
                        usbPaths.push_back(entry.path().string());
                    }
                }
            } catch (const std::exception& e) {
                std::cerr << "Error accessing USB devices: " << e.what() << std::endl;
                return;
            }

            // Hiển thị danh sách các USB
            if (usbPaths.empty()) {
                std::cout << "No USB devices found in " << usbRootPath << std::endl;
                return;
            }
            //clearTerminal();
            std::cout << "\n==== Available USB Devices ====\n";
            for (size_t i = 0; i < usbPaths.size(); ++i) {
                std::cout << i + 1 << ". " << usbPaths[i] << std::endl;
            }

            // Nhập chỉ số từ người dùng
            int usbIndex = -1;
            std::cout << "\nSelect a USB device by index: ";
            std::cin >> usbIndex;
            std::cin.ignore(INT_MAX, '\n');

            // Kiểm tra chỉ số hợp lệ
            if (usbIndex < 1 || usbIndex > static_cast<int>(usbPaths.size())) {
                std::cerr << "Invalid index selected!" << std::endl;
                return;
            }

            // Lấy đường dẫn của USB được chọn
            std::string selectedUsbPath = usbPaths[usbIndex - 1];
            std::cout << "Scanning USB: " << selectedUsbPath << std::endl;

            // Quét thư mục USB
            scanDirectory(selectedUsbPath);
            break;
        }
        default:
            std::cerr << "Invalid scan option!" << std::endl;
            break;
    }
}


void MediaFileController::scanAndDisplayMedia() {
    //clearTerminal();
    MediaFileView* mediaFileView = dynamic_cast<MediaFileView*>(ManagerController::getInstance().getManagerView()->getView());
    if (!mediaFileView) {
        std::cerr << "Error: MediaFileView is null!" << std::endl;
        return;
    }

    // Access media library and initialize pagination
    auto& mediaLibrary = ManagerController::getInstance().getManagerModel()->getMediaLibrary();
    int totalPages = mediaLibrary.getTotalPages(pageSize);

    // Get MediaFile objects and convert to strings for display
    auto files = mediaLibrary.getMediaFilesForPage(0, pageSize);
    std::vector<std::string> fileStrings;

    for (const auto& file : files) {
    fileStrings.push_back(std::to_string(file.getIndex()) + ". " + file.getName());
    }

    // Display first page of media files
    mediaFileView->displayMediaFiles(fileStrings, 1);
    mediaFileView->displayPagination(1, mediaLibrary.getTotalPages(pageSize));
    // Exit after displaying the first page
    return;
}




void MediaFileController::nextPage() {
    MediaFileView* mediaFileView = dynamic_cast<MediaFileView*>(ManagerController::getInstance().getManagerView()->getView());
    if (!mediaFileView) {
        std::cerr << "Error: MediaFileView is null!" << std::endl;
        return;
    }

    auto& mediaLibrary =  ManagerController::getInstance().getManagerModel()->getMediaLibrary();
    if (currentPage + 1 < mediaLibrary.getTotalPages(pageSize)) {
        currentPage++;
        auto files = mediaLibrary.getMediaFilesForPage(currentPage, pageSize);
        std::vector<std::string> fileStrings;
        for (const auto& file : files) {
            fileStrings.push_back(std::to_string(file.getIndex()) + ". " + file.getName());
        }

        mediaFileView->displayMediaFiles(fileStrings, currentPage + 1);
        mediaFileView->displayPagination(currentPage + 1, mediaLibrary.getTotalPages(pageSize));
    } else {
        std::cout << "Already on the last page.\n";
    }
}

void MediaFileController::previousPage() {
    MediaFileView* mediaFileView = dynamic_cast<MediaFileView*>(ManagerController::getInstance().getManagerView()->getView());
    if (!mediaFileView) {
        std::cerr << "Error: MediaFileView is null!" << std::endl;
        return;
    }

    auto& mediaLibrary = ManagerController::getInstance().getManagerModel()->getMediaLibrary();
    if (currentPage > 0) {
        currentPage--;
        auto files = mediaLibrary.getMediaFilesForPage(currentPage, pageSize);
        std::vector<std::string> fileStrings;
        for (const auto& file : files) {
            fileStrings.push_back(std::to_string(file.getIndex()) + ". " + file.getName());
        }

        mediaFileView->displayMediaFiles(fileStrings, currentPage + 1);
        mediaFileView->displayPagination(currentPage + 1, mediaLibrary.getTotalPages(pageSize));
    } else {
        std::cout << "Already on the first page.\n";
    }
}



std::string MediaFileController::getPathById(const std::vector<MediaFile>& mediaFiles, int id) {
    for (const auto& mediaFile : mediaFiles) {
        if (mediaFile.getIndex() == id) { 
            return mediaFile.getPath();   
        }
    }
    return ""; 
}

void MediaFileController::handleAction(int action) {
    switch (action) {
    case 1:{
        int mediaId;
        std::cout << "\nEnter Media ID to show properties: ";
        std::cin >> mediaId;
        std::cin.ignore(INT_MAX, '\n');
        //getPath 
        auto& mediaFiles = ManagerController::getInstance().getManagerModel()->getMediaLibrary().getMediaFiles();
        std::string filepath = getPathById(mediaFiles, mediaId);
    
         // Chuyển View sang MetadataView
        ManagerController::getInstance().getManagerView()->setView("Metadata");

        MetadataController* metadataController = dynamic_cast<MetadataController*>(ManagerController::getInstance().getController("Metadata"));
        if (!metadataController) {
            std::cerr << "Error: MetadataController is not available!" << std::endl;
            break;
        }
        
        // Gọi logic hiển thị metadata từ MetadataController
        std::cout << "\nShowing Metadata..." << std::endl;
        metadataController->handleShowMetadata(filepath);
        ManagerController::getInstance().getManagerView()->setView("Metadata");
        
        break;
    }
    case 2:
        std::cout << "\nGoing to Next Page..." << std::endl;
        clearTerminal();
        nextPage();
        break;
    case 3:
        std::cout << "\nGoing to Previous Page..." << std::endl;
        clearTerminal();
        previousPage();
        break;
    case 4:
        int mediaId;
        std::cout << "\nEnter Media ID to Play: ";
        std::cin >> mediaId;
        std::cout << "\nPlaying Media with ID: " << mediaId << std::endl;
        //model->getMediaLibrary().playMedia(mediaId);
        break;
    case 0:
        std::cout << "\nReturning Home...\n";
        ManagerController::getInstance().getManagerView()->setView("Default");
        //system("clear");
        break;
    default:
        std::cout << "Invalid choice! Please try again." << std::endl;
        break;
    }
}

