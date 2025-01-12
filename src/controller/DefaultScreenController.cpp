#include "controller/DefaultScreenController.h"
#include "controller/ManagerController.h" 
#include "controller/MediaFileController.h"
#include "controller/PlaylistController.h"
#include <iostream>
DefaultScreenController::DefaultScreenController(){}



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
        std::string directoryPath = mediaFileView->promptDirectoryInput();
        mediaFileController->scanDirectory(directoryPath);
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

    case 3:
        std::cout << "\nSwitching to Now Playing View..." << std::endl;
        ManagerController::getInstance().getManagerView()->setView("NowPlaying");
        break;
    case 4:
        std::cout << "\nExiting the application..." << std::endl;
        exit(0);
    default:
        std::cout << "Invalid choice! Please try again." << std::endl;
    }
}
