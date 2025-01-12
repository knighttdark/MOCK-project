#include "controller/DefaultScreenController.h"
#include "controller/ManagerController.h"
#include "controller/MediaFileController.h"
#include <iostream>

DefaultScreenController::DefaultScreenController(){}

void DefaultScreenController::handleAction(int action) {
    switch (action) {
    case BROWSE_MEDIA_FILES: {
        MediaFileController* mediaFileController = dynamic_cast<MediaFileController*>(
            ManagerController::getInstance().getController("MediaFile"));

        if (!mediaFileController) {
            cerr << "Error: MediaFileController is not available!" << endl;
            break;
        }
        ManagerController::getInstance().getManagerView()->setView("MediaFile");
        MediaFileView* mediaFileView = dynamic_cast<MediaFileView*>(
            ManagerController::getInstance().getManagerView()->getView());
        if (!mediaFileView) {
            cerr << "Error: MediaFileView is not available! Current View Key: "
                      << ManagerController::getInstance().getManagerView()->getCurrentViewKey() << endl;
            break;
        }

        cout << "\nSwitching to Media File View..." << endl;
        string directoryPath = mediaFileView->promptDirectoryInput();
        mediaFileController->scanDirectory(directoryPath);
        mediaFileController->scanAndDisplayMedia();
        break;
    }
    case PLAYLIST:
        cout << "\nSwitching to Playlist View..." << endl;
        ManagerController::getInstance().getManagerView()->setView("Playlist");
        break;
    case NOW_PLAYING:
        cout << "\nSwitching to Now Playing View..." << endl;
        ManagerController::getInstance().getManagerView()->setView("NowPlaying");
        break;
    case EXIT:
        cout << "\nExiting the application..." << endl;
        exit(0);
    default:
        cout << "Invalid choice! Please try again." << endl;
    }
}
