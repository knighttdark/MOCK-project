#include "controller/DefaultScreenController.h"
#include "controller/ManagerController.h"
#include "controller/MediaFileController.h"
#include "controller/PlaylistController.h"
#include "view/PlaylistView.h"
#include "common/Enum.h"
#include <iostream>
#include "common/TerminalUtils.h"
#include "controller/PlayingMediaController.h"
#include "gmock/gmock.h"

/* Default constructor for DefaultScreenController */
//DefaultScreenController::DefaultScreenController() {}
// DefaultScreenController::DefaultScreenController(ManagerController* controller) {
//     if (controller) {
//         managerController = controller;
//     } else {
//         managerController = &ManagerController::getInstance();
//     }
// }

/* Function to handle user actions in the default screen view */
// void DefaultScreenController::handleAction(int action) {
//     switch (action) {
//         case ACTION_SWITCH_TO_MEDIA_FILE: {
//             /* Switch to the Media File view */
//             MediaFileController* mediaFileController = dynamic_cast<MediaFileController*>(
//                 ManagerController::getInstance().getController("MediaFile"));
//             if (!mediaFileController) {
//                 cerr << "Error: MediaFileController is not available!" << endl;
//                 break;
//             }

//             /* Set the Media File view */
//             ManagerController::getInstance().getManagerView()->setView("MediaFile");

//             /* Display the Media File view */
//             MediaFileView* mediaFileView = dynamic_cast<MediaFileView*>(
//                 ManagerController::getInstance().getManagerView()->getView());
//             if (!mediaFileView) {
//                 cerr << "Error: MediaFileView is not available! Current View Key: "
//                     << ManagerController::getInstance().getManagerView()->getCurrentViewKey() << endl;
//                 break;
//             }

//             /* Show the Media File view */
//             int option = mediaFileView->showOptionScan();
//             mediaFileController->handleActionScan(option);
//             clearTerminal();
//             mediaFileController->scanAndDisplayMedia();
//             break;
//         }
//         case ACTION_SWITCH_TO_PLAYLIST: {
//             /* Switch to the Playlist view */
//             PlaylistController* playlistController = dynamic_cast<PlaylistController*>(
//                 ManagerController::getInstance().getController("Playlist"));
//             if (!playlistController) {
//                 cerr << "Error: PlaylistController is not available!" << endl;
//                 break;
//             }

//             /* Set the Playlist view */
//             ManagerController::getInstance().getManagerView()->setView("Playlist");

//             /* Display the Playlist view */
//             PlaylistView* playlistView = dynamic_cast<PlaylistView*>(
//                 ManagerController::getInstance().getManagerView()->getView());
//             if (!playlistView) {
//                 cerr << "Error: PlaylistView is not available!" << endl;
//                 break;
//             }

//             /* Show the Playlist view */
//             clearTerminal();
//             playlistController->listAllPlaylists();
//             break;
//         }
//         case ACTION_EXIT_APPLICATION: {
//             /* Stop the current media and exit the application */
//             PlayingMediaController* playingController = dynamic_cast<PlayingMediaController*>(
//                 ManagerController::getInstance().getController("PlayingView"));

//             if (!playingController) {
//                 break;}

//             /* Stop the current media and return to the home screen */
//             playingController->stop();
//             exit(0);
//             break;
//         }
//         default:
//             cout << "Invalid choice! Please try again." << endl;
//             break;
//     }
// }
DefaultScreenController::DefaultScreenController() {
    managerController = &ManagerController::getInstance();
}
void DefaultScreenController::handleAction(int action) {
    switch (action) {
        case ACTION_SWITCH_TO_MEDIA_FILE: {
            /* Switch to the Media File view */
            MediaFileController* mediaFileController = dynamic_cast<MediaFileController*>(
                managerController->getController("MediaFile"));  

            /* Set the Media File view */
            managerController->getManagerView()->setView("MediaFile");  

            /* Display the Media File view */
            MediaFileView* mediaFileView = dynamic_cast<MediaFileView*>(
                managerController->getManagerView()->getView());  

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
                managerController->getController("Playlist"));  

            /* Set the Playlist view */
            managerController->getManagerView()->setView("Playlist");  

            /* Display the Playlist view */
            PlaylistView* playlistView = dynamic_cast<PlaylistView*>(
                managerController->getManagerView()->getView());  

            /* Show the Playlist view */
            clearTerminal();
            playlistController->listAllPlaylists();
            break;
        }
        case ACTION_EXIT_APPLICATION: {
            PlayingMediaController* playingController = dynamic_cast<PlayingMediaController*>(
                managerController->getController("PlayingView"));

            /* Stop the current media */
            playingController->stop();

            /* ✅ Thêm VerifyAndClear để tránh mock leak */
           if (!isTestMode) {  // ✅ Chỉ gọi exit(0) nếu không phải test mode
            exit(0);
            }


            //exit(0); 
            break;
        }

        default:
            cout << "Invalid choice! Please try again." << endl;
            break;
    }
}