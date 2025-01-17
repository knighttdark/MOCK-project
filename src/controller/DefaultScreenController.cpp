#include "controller/DefaultScreenController.h"
#include "controller/ManagerController.h"
#include "controller/MediaFileController.h"
#include "controller/PlaylistController.h"
#include "view/PlaylistView.h"
#include "common/Enum.h"
#include <iostream>

/* Constructor for DefaultScreenController */
DefaultScreenController::DefaultScreenController() {}

/* Handle actions based on the input action code */
void DefaultScreenController::handleAction(int action) {
    switch (action) {

    /* Case: Switch to Media File View */
    case ACTION_SWITCH_TO_MEDIA_FILE: {
        /* Retrieve the MediaFileController instance */
        MediaFileController* mediaFileController = dynamic_cast<MediaFileController*>(
            ManagerController::getInstance().getController("MediaFile"));

        /* Check if the MediaFileController is available */
        if (!mediaFileController) {
            cerr << "Error: MediaFileController is not available!" << endl;
            break;
        }

        /* Set the current view to Media File */
        ManagerController::getInstance().getManagerView()->setView("MediaFile");

        /* Retrieve the MediaFileView instance */
        MediaFileView* mediaFileView = dynamic_cast<MediaFileView*>(
            ManagerController::getInstance().getManagerView()->getView());

        /* Check if the MediaFileView is available */
        if (!mediaFileView) {
            cerr << "Error: MediaFileView is not available! Current View Key: "
                 << ManagerController::getInstance().getManagerView()->getCurrentViewKey() << endl;
            break;
        }

        /* Display a message indicating the view switch */
        cout << "\nSwitching to Media File View..." << endl;

        /* Show scanning options in the Media File View */
        /* Handle user input for the scanning option */
        int option = mediaFileView->showOptionScan();

        /* Handle the scanning process based on user input */
        mediaFileController->handleActionScan(option);

        /* Scan and display media files */
        mediaFileController->scanAndDisplayMedia();
        break;
    }

    /* Case: Switch to Playlist View */
    case ACTION_SWITCH_TO_PLAYLIST: {
        /* Retrieve the PlaylistController instance */
        PlaylistController* playlistController = dynamic_cast<PlaylistController*>(
            ManagerController::getInstance().getController("Playlist"));

        /* Check if the PlaylistController is available */
        if (!playlistController) {
            cerr << "Error: PlaylistController is not available!" << endl;
            break;
        }

        /* Set the current view to Playlist */
        ManagerController::getInstance().getManagerView()->setView("Playlist");

        /* Retrieve the PlaylistView instance */
        PlaylistView* playlistView = dynamic_cast<PlaylistView*>(
            ManagerController::getInstance().getManagerView()->getView());

        /* Check if the PlaylistView is available */
        if (!playlistView) {
            cerr << "Error: PlaylistView is not available!" << endl;
            break;
        }

        /* Display a message indicating the view switch */
        cout << "\nSwitching to Playlist View..." << endl;

        /* List all available playlists */
        playlistController->listAllPlaylists();
        break;
    }

    /* Case: Exit the application */
    case ACTION_EXIT_APPLICATION: {
        /* Display a message indicating the application exit */
        cout << "\nExiting the application..." << endl;
        exit(0);
        break;
    }

    /* Default case: Handle invalid choices */
    default:
        cout << "Invalid choice! Please try again." << endl;
        break;
    }
}
