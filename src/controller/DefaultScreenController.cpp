#include "controller/DefaultScreenController.h"
#include "controller/ManagerController.h"
#include "controller/MediaFileController.h"
#include "controller/PlaylistController.h"
#include "view/PlaylistView.h"
#include "common/Enum.h"
#include <iostream>
#include "common/TerminalUtils.h"
#include "controller/PlayingMediaController.h"

/* Default constructor for DefaultScreenController */
DefaultScreenController::DefaultScreenController() {}

/* Function to handle user actions in the default screen view */
void DefaultScreenController::handleAction(int action) {
    switch (action) {
        case ACTION_SWITCH_TO_MEDIA_FILE: {
            /* Switch to the Media File view */
            MediaFileController* mediaFileController = dynamic_cast<MediaFileController*>(
                ManagerController::getInstance().getController("MediaFile"));
            if (!mediaFileController) {
                cerr << "Error: MediaFileController is not available!" << endl;
                break;
            }

            /* Set the Media File view */
            ManagerController::getInstance().getManagerView()->setView("MediaFile");

            /* Display the Media File view */
            MediaFileView* mediaFileView = dynamic_cast<MediaFileView*>(
                ManagerController::getInstance().getManagerView()->getView());
            if (!mediaFileView) {
                cerr << "Error: MediaFileView is not available! Current View Key: "
                    << ManagerController::getInstance().getManagerView()->getCurrentViewKey() << endl;
                break;
            }

            /* Show the Media File view */
            int option = mediaFileView->showOptionScan();
            mediaFileController->handleActionScan(option);
            clearTerminal();
            mediaFileController->scanAndDisplayMedia();
            break;
        }
        case ACTION_SWITCH_TO_PLAYLIST: {
            /* Switch to the Playlist view */
            PlaylistController* playlistController = dynamic_cast<PlaylistController*>(
                ManagerController::getInstance().getController("Playlist"));
            if (!playlistController) {
                cerr << "Error: PlaylistController is not available!" << endl;
                break;
            }

            /* Set the Playlist view */
            ManagerController::getInstance().getManagerView()->setView("Playlist");

            /* Display the Playlist view */
            PlaylistView* playlistView = dynamic_cast<PlaylistView*>(
                ManagerController::getInstance().getManagerView()->getView());
            if (!playlistView) {
                cerr << "Error: PlaylistView is not available!" << endl;
                break;
            }

            /* Show the Playlist view */
            clearTerminal();
            playlistController->listAllPlaylists();
            break;
        }
        case ACTION_EXIT_APPLICATION: {
            /* Stop the current media and exit the application */
            PlayingMediaController* playingController = dynamic_cast<PlayingMediaController*>(
                ManagerController::getInstance().getController("PlayingView"));

            if (!playingController) {
                cerr << "Error: PlayingMediaController not available!\n";
                break;}

            /* Stop the current media and return to the home screen */
            playingController->stop();
            exit(0);
            break;
        }
        default:
            cout << "Invalid choice! Please try again." << endl;
            break;
    }
}
