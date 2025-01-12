#include "controller/DefaultScreenController.h"
#include "controller/ManagerController.h"
#include "controller/MediaFileController.h"
#include "controller/PlaylistController.h"
#include "view/PlaylistView.h"
#include <iostream>

DefaultScreenController::DefaultScreenController() {}

void DefaultScreenController::handleAction(int action) {
    switch (action) {
    case 1: {
        MediaFileController* mediaFileController = dynamic_cast<MediaFileController*>(
            ManagerController::getInstance().getController("MediaFile"));

        if (!mediaFileController) {
            std::cerr << "Error: MediaFileController is not available!" << std::endl;
            break;
        }

        ManagerController::getInstance().getManagerView()->setView("MediaFile");
        MediaFileView* mediaFileView = dynamic_cast<MediaFileView*>(
            ManagerController::getInstance().getManagerView()->getView());
        if (!mediaFileView) {
            std::cerr << "Error: MediaFileView is not available! Current View Key: " 
                      << ManagerController::getInstance().getManagerView()->getCurrentViewKey() << std::endl;
            break;
        }

        std::cout << "\nSwitching to Media File View..." << std::endl;
        mediaFileView->showOptionScan();
        int option = mediaFileView->handleInputOptionScan();

        // Xử lý quét dựa trên lựa chọn
        mediaFileController->handleActionScan(option);

        // Hiển thị các tệp media sau khi quét
        mediaFileController->scanAndDisplayMedia();
        break;
    }

    case 2: {
    PlaylistController* playlistController = dynamic_cast<PlaylistController*>(
        ManagerController::getInstance().getController("Playlist"));

    if (!playlistController) {
        std::cerr << "Error: PlaylistController is not available!" << std::endl;
        break;
    }
    ManagerController::getInstance().getManagerView()->setView("Playlist");
    PlaylistView* playlistView = dynamic_cast<PlaylistView*>(
        ManagerController::getInstance().getManagerView()->getView());
    if (!playlistView) {
        std::cerr << "Error: PlaylistView is not available!" << std::endl;
        break;
    }

    std::cout << "\nSwitching to Playlist View..." << std::endl;

    // Hiển thị danh sách playlist hiện tại
    playlistController->listAllPlaylists();
    break;
}

    case 0: {
        std::cout << "\nExiting the application..." << std::endl;
        exit(0);
        break;
    }

    default:
        std::cout << "Invalid choice! Please try again." << std::endl;
        break;
    }
}
