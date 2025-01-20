#include "controller/DefaultScreenController.h"
#include "controller/ManagerController.h"
#include "controller/MediaFileController.h"
#include "controller/PlaylistController.h"
#include "view/PlaylistView.h"
#include "common/Enum.h"
#include <iostream>


DefaultScreenController::DefaultScreenController() {}


void DefaultScreenController::handleAction(int action) {
    switch (action) {

    
    case ACTION_SWITCH_TO_MEDIA_FILE: {
        
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

        
        
        int option = mediaFileView->showOptionScan();

        
        mediaFileController->handleActionScan(option);

        
        mediaFileController->scanAndDisplayMedia();
        break;
    }

    
    case ACTION_SWITCH_TO_PLAYLIST: {
        
        PlaylistController* playlistController = dynamic_cast<PlaylistController*>(
            ManagerController::getInstance().getController("Playlist"));

        
        if (!playlistController) {
            cerr << "Error: PlaylistController is not available!" << endl;
            break;
        }

        
        ManagerController::getInstance().getManagerView()->setView("Playlist");

        
        PlaylistView* playlistView = dynamic_cast<PlaylistView*>(
            ManagerController::getInstance().getManagerView()->getView());

        
        if (!playlistView) {
            cerr << "Error: PlaylistView is not available!" << endl;
            break;
        }

        
        cout << "\nSwitching to Playlist View..." << endl;

        
        playlistController->listAllPlaylists();
        break;
    }

    
    case ACTION_EXIT_APPLICATION: {
        
        cout << "\nExiting the application..." << endl;
        exit(0);
        break;
    }

    
    default:
        cout << "Invalid choice! Please try again." << endl;
        break;
    }
}
