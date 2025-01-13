
#include "controller/MediaFileController.h"
#include "controller/ManagerController.h"
#include "model/PlaylistLibrary.h"
#include <common/TerminalUtils.h>
#include <common/Enum.h>
#include <stdexcept>
#include <iostream>
#include <climits>

/* Constructor for MediaFileController */
MediaFileController::MediaFileController(){}

/* Scans a given directory and adds the media files to the media library */
void MediaFileController::scanDirectory(const string& path) {
    try {
        ManagerController::getInstance().getManagerModel()->getMediaLibrary().scanDirectory(path);
        cout << "Directory scanned successfully.\n";
    } catch (const exception& e) {
        cerr << "Error scanning directory: " << e.what() << endl;
    }
}


/* Handles the scan action based on the user's choice */
void MediaFileController::handleActionScan(int option) {
    MediaFileView* mediaFileView = dynamic_cast<MediaFileView*>(ManagerController::getInstance().getManagerView()->getView());
    if (!mediaFileView) {
        cerr << "Error: MediaFileView is null!" << endl;
        return;
    }

    switch (option) {
        case SCAN_DIRECTORY: {
            /* Scan a specific directory */
            string directoryPath = mediaFileView->promptDirectoryInput();
            scanDirectory(directoryPath);
            break;
        }
        case SCAN_USB: {
            /* Scan USB devices */
            string usbRootPath = "/media/kist";
            vector<string> usbPaths;

            /* Iterate through the USB root path to find connected USB devices */
            try {
                for (const auto& entry : filesystem::directory_iterator(usbRootPath)) {
                    if (entry.is_directory()) {
                        usbPaths.push_back(entry.path().string());
                    }
                }
            } catch (const exception& e) {
                cerr << "Error accessing USB devices: " << e.what() << endl;
                return;
            }

            /* Display available USB devices */
            if (usbPaths.empty()) {
                cout << "No USB devices found in " << usbRootPath << endl;
                return;
            }

            cout << "\n==== Available USB Devices ====\n";
            for (size_t i = 0; i < usbPaths.size(); ++i) {
                cout << i + 1 << ". " << usbPaths[i] << endl;
            }

            /* Get the user's choice of USB device */
            int usbIndex = -1;
            cout << "\nSelect a USB device by index: ";
            cin >> usbIndex;
            cin.ignore(INT_MAX, '\n');

            /* Validate the selected index */
            if (usbIndex < 1 || usbIndex > static_cast<int>(usbPaths.size())) {
                cerr << "Invalid index selected!" << endl;
                return;
            }

            /* Scan the selected USB directory */
            string selectedUsbPath = usbPaths[usbIndex - 1];
            cout << "Scanning USB: " << selectedUsbPath << endl;
            scanDirectory(selectedUsbPath);
            break;
        }
        default:
            cerr << "Invalid scan option!" << endl;
            break;
    }
}

/* Displays the media files in the library with pagination */
void MediaFileController::scanAndDisplayMedia() {
    MediaFileView* mediaFileView = dynamic_cast<MediaFileView*>(ManagerController::getInstance().getManagerView()->getView());
    if (!mediaFileView) {
        cerr << "Error: MediaFileView is null!" << endl;
        return;
    }

    /* Access the media library to initialize pagination */
    auto& mediaLibrary = ManagerController::getInstance().getManagerModel()->getMediaLibrary();
    int totalPages = mediaLibrary.getTotalPages(pageSize);

    /* Retrieve the media files for the first page */
    auto files = mediaLibrary.getMediaFilesForPage(0, pageSize);
    vector<string> fileStrings;

    for (const auto& file : files) {
        fileStrings.push_back(to_string(file.getIndex()) + ". " + file.getName());
    }

    /* Display the first page of media files */
    mediaFileView->displayMediaFiles(fileStrings, 1);
    mediaFileView->displayPagination(1, totalPages);
}

/* Displays the next page of media files */
void MediaFileController::nextPage() {
    MediaFileView* mediaFileView = dynamic_cast<MediaFileView*>(ManagerController::getInstance().getManagerView()->getView());
    if (!mediaFileView) {
        cerr << "Error: MediaFileView is null!" << endl;
        return;
    }

    /* Access the media library and go to the next page if available */
    auto& mediaLibrary = ManagerController::getInstance().getManagerModel()->getMediaLibrary();
    if (currentPage + 1 < mediaLibrary.getTotalPages(pageSize)) {
        currentPage++;
        auto files = mediaLibrary.getMediaFilesForPage(currentPage, pageSize);
        vector<string> fileStrings;

        for (const auto& file : files) {
            fileStrings.push_back(to_string(file.getIndex()) + ". " + file.getName());
        }

        mediaFileView->displayMediaFiles(fileStrings, currentPage + 1);
        mediaFileView->displayPagination(currentPage + 1, mediaLibrary.getTotalPages(pageSize));
    } else {
        cout << "Already on the last page.\n";
    }
}

/* Displays the previous page of media files */
void MediaFileController::previousPage() {
    MediaFileView* mediaFileView = dynamic_cast<MediaFileView*>(ManagerController::getInstance().getManagerView()->getView());
    if (!mediaFileView) {
        cerr << "Error: MediaFileView is null!" << endl;
        return;
    }

    /* Access the media library and go to the previous page if available */
    auto& mediaLibrary = ManagerController::getInstance().getManagerModel()->getMediaLibrary();
    if (currentPage > 0) {
        currentPage--;
        auto files = mediaLibrary.getMediaFilesForPage(currentPage, pageSize);
        vector<string> fileStrings;

        for (const auto& file : files) {
            fileStrings.push_back(to_string(file.getIndex()) + ". " + file.getName());
        }

        mediaFileView->displayMediaFiles(fileStrings, currentPage + 1);
        mediaFileView->displayPagination(currentPage + 1, mediaLibrary.getTotalPages(pageSize));
    } else {
        cout << "Already on the first page.\n";
    }
}

/* Retrieves the file path of a media file by its ID */
string MediaFileController::getPathById(const vector<MediaFile>& mediaFiles, int id) {
    for (const auto& mediaFile : mediaFiles) {
        if (mediaFile.getIndex() == id) {
            return mediaFile.getPath();
        }
    }
    return "";
}

/* Handles various actions based on user input */
void MediaFileController::handleAction(int action) {
    switch (action) {
        case ACTION_SHOW_PROPERTIES:{
            int mediaId;
            cout << "\nEnter Media ID to show properties: ";
            cin >> mediaId;
            cin.ignore(INT_MAX, '\n');
            /* getPath */
            auto& mediaFiles = ManagerController::getInstance().getManagerModel()->getMediaLibrary().getMediaFiles();
            string filepath = getPathById(mediaFiles, mediaId);

            /* Switch to Metadata View */
            ManagerController::getInstance().getManagerView()->setView("Metadata");

            MetadataController* metadataController = dynamic_cast<MetadataController*>(ManagerController::getInstance().getController("Metadata"));
            if (!metadataController) {
                cerr << "Error: MetadataController is not available!" << endl;
                break;
            }

            /* Show metadata using MetadataController */
            cout << "\nShowing Metadata..." << endl;
            metadataController->handleShowMetadata(filepath);
            ManagerController::getInstance().getManagerView()->setView("Metadata");

            break;
        }
        case ACTION_NEXT_PAGE:
            cout << "\nGoing to Next Page..." << endl;
            clearTerminal();
            nextPage();
            break;
        case ACTION_PREVIOUS_PAGE:
            cout << "\nGoing to Previous Page..." << endl;
            clearTerminal();
            previousPage();
            break;
        case ACTION_PLAY_MEDIA:
            /* Play media file */
            int mediaId;
            cout << "\nEnter Media ID to Play: ";
            cin >> mediaId;
            cout << "\nPlaying Media with ID: " << mediaId << endl;
            //model->getMediaLibrary().playMedia(mediaId);
            break;

        case ACTION_ADD_TO_PLAYLIST: {
            /* Add media file to playlist */
            int mediaId;
            cout << "\nEnter Media ID to add to a playlist: ";
            cin >> mediaId;

            /* Get the list of media files */
            auto& mediaLibrary = ManagerModel::getInstance().getMediaLibrary();
            const auto& mediaFiles = mediaLibrary.getMediaFiles();

            /* Find the media file by ID */
            auto it = find_if(mediaFiles.begin(), mediaFiles.end(),
                [mediaId](const MediaFile& file) { return file.getIndex() == mediaId; });

            if (it == mediaFiles.end()) {
                cerr << "Error: Invalid Media ID!\n";
                break;
            }

            /* Get the list of playlists from PlaylistLibrary */
            PlaylistLibrary& playlistLibrary = ManagerModel::getInstance().getPlaylistLibrary();
            auto& playlists = playlistLibrary.getPlaylists();

            /* Check if there are no playlists available */
            if (playlists.empty()) {
                cout << "No playlists available.\n";
                break;
            }

            /* Display the list of playlists */
            ManagerController::getInstance().getManagerView()->setView("Playlist");
            PlaylistView* playlistView = dynamic_cast<PlaylistView*>(ManagerController::getInstance().getManagerView()->getView());
            if (!playlistView) {
                cerr << "Error: PlaylistView is not available!\n";
                break;
            }
            playlistView->displayPlaylists(playlists);

            /* Allow the user to select a playlist by ID */
            int playlistId;
            cout << "\nEnter Playlist ID to add media '" << it->getName() << "': ";
            cin >> playlistId;

            /* Validate the selected playlist ID */
            if (playlistId <= 0 || playlistId > static_cast<int>(playlists.size())) {
                cerr << "Error: Invalid Playlist ID!\n";
                break;
            }

            /* Get the selected playlist */
            Playlist& selectedPlaylist = playlists[playlistId - 1];

            /* Add the media file to the selected playlist */
            selectedPlaylist.addSong(*it);
            cout << "Media file '" << it->getName() << "' added to playlist '" << selectedPlaylist.getName() << "'.\n";

            /* Save the updated playlist to the file */
            try {
                playlistLibrary.saveToFile("playlists.txt");
                cout << "Updated playlist saved to file successfully.\n";
            } catch (const exception& e) {
                cerr << "Error saving playlist to file: " << e.what() << '\n';
            }

            /* Switch back to MediaFile view */
            MediaFileController* mediaFileController = dynamic_cast<MediaFileController*>(
                ManagerController::getInstance().getController("MediaFile"));

            if (!mediaFileController) {
                cerr << "Error: MediaFileController is not available!" << endl;
                break;
            }
            ManagerController::getInstance().getManagerView()->setView("MediaFile");
            cout << "\nSwitching to Media File View..." << endl;
            mediaFileController->scanAndDisplayMedia();

            break;
            }

        case ACTION_RETURN_HOME:
            cout << "\nReturning Home...\n";
            ManagerController::getInstance().getManagerView()->setView("Default");
            //system("clear");
            break;
        default:
            cout << "Invalid choice! Please try again." << endl;
            break;
    }
}

