
#include "controller/MediaFileController.h"
#include "controller/ManagerController.h"
#include "controller/PlayingMediaController.h"
#include "model/PlaylistLibrary.h"
#include "view/PlayingView.h"
#include <common/TerminalUtils.h>
#include <common/Enum.h>
#include <stdexcept>
#include <iostream>
#include <climits>

/* Default constructor for MediaFileController */
MediaFileController::MediaFileController(){}

/* Function to scan a directory and display the result in the terminal */
void MediaFileController::scanDirectory(const string& path) {
    string notification_message;
    bool success = false;

    try {
        /* Scan the directory using ManagerController */
        ManagerController::getInstance().getManagerModel()->getMediaLibrary().scanDirectory(path);
        notification_message = "Directory '" + path + "' scanned successfully.";
        success = true;
    } catch (const exception& e) {
        notification_message = "Error scanning directory: " + string(e.what());
        success = false;
    }

    /* Create an interactive screen to display the result */
    auto screen = ScreenInteractive::TerminalOutput();

    auto renderer = Renderer([&] {
        return vbox({
            text("Directory Scan Result") | bold | center,
            separator(),
            text(notification_message) | (success ? color(Color::Green) : color(Color::Red)) | center,
            separator(),
            text("Press ENTER to return.") | dim | center
        }) | center;
    });

    /* Catch user input to exit the interactive screen */
    auto main_component = CatchEvent(renderer, [&](Event event) {
        if (event == Event::Return) {
            screen.ExitLoopClosure()();
            return true;
        }
        return false;
    });

    screen.Loop(main_component);
}

void MediaFileController::handleActionScan(int option) {
    MediaFileView* mediaFileView = dynamic_cast<MediaFileView*>(ManagerController::getInstance().getManagerView()->getView());
    if (!mediaFileView) {
        cerr << "Error: MediaFileView is null!" << endl;
        return;
    }

    switch (option) {
        case SCAN_DIRECTORY: {
            /* Handle scanning a user-specified directory */
            string directoryPath = mediaFileView->promptDirectoryInput();
            scanDirectory(directoryPath);
            break;
        }
        case SCAN_USB: {
            /* Handle scanning connected USB devices */
            string usbRootPath;

            try {
                /* Retrieve the root path for USB devices */
                char* username = getenv("USER");
                if (username == nullptr) {
                    cerr << "Error: Could not retrieve current username." << endl;
                    return;
                }

                usbRootPath = "/media/" + string(username);
            } catch (const exception& e) {
                cerr << "Error retrieving USB root path: " << e.what() << endl;
                return;
            }

            /* Collect available USB directories */
            vector<string> usbPaths;

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

            if (usbPaths.empty()) {
                cout << "No USB devices found in " << usbRootPath << endl;
                return;
            }

            /* Allow user to select a USB device and scan it */
            int selected = 0;
            string error_message;
            vector<string> usbEntries;
            for (size_t i = 0; i < usbPaths.size(); ++i) {
                usbEntries.push_back(to_string(i + 1) + ". " + usbPaths[i]);
            }

            auto screen = ScreenInteractive::TerminalOutput();
            auto menu = Menu(&usbEntries, &selected);

            auto renderer = Renderer(menu, [&] {
                return vbox({
                    text("==== Available USB Devices ====") | bold | center,
                    separator(),
                    menu->Render() | border,
                    separator(),
                    text("Use UP/DOWN keys, numbers (1-" + to_string(usbEntries.size()) + "), or click to select.") | dim | center,
                    separator(),
                    text(error_message) | color(Color::Red) | center
                });
            });

            /* Run the menu loop to let the user choose a USB device */
            screen.Loop(renderer | CatchEvent([&](Event event) {
                if (event == Event::Return) {
                    if (selected >= 0 && selected < usbPaths.size()) {
                        string selectedUsbPath = usbPaths[selected];
                        cout << "Scanning USB: " << selectedUsbPath << endl;
                        scanDirectory(selectedUsbPath);
                    }
                    screen.ExitLoopClosure()();
                    return true;
                }

                if (event.is_mouse() && event.mouse().button == Mouse::Left) {
                    int clicked_index = event.mouse().y - 3;
                    if (clicked_index >= 0 && clicked_index < usbEntries.size()) {
                        selected = clicked_index;
                        string selectedUsbPath = usbPaths[selected];
                        cout << "Scanning USB: " << selectedUsbPath << endl;
                        scanDirectory(selectedUsbPath);
                        screen.ExitLoopClosure()();
                    }
                    return true;
                }

                return false;
            }));
            break;
        }

        case RETURN_HOME:{
            /* Handle returning to the home screen */
            ManagerController::getInstance().getManagerView()->setView("Default");
            break;
        }
        default:
            cerr << "Invalid scan option!" << endl;
            break;
    }
}

/* Function to scan and display media files in the current page */
void MediaFileController::scanAndDisplayMedia() {
    MediaFileView* mediaFileView = dynamic_cast<MediaFileView*>(ManagerController::getInstance().getManagerView()->getView());
    if (!mediaFileView) {
        cerr << "Error: MediaFileView is null!" << endl;
        return;
    }

    /* Retrieve media files for the first page */
    auto& mediaLibrary = ManagerController::getInstance().getManagerModel()->getMediaLibrary();
    string notification_message = "";

    auto files = mediaLibrary.getMediaFilesForPage(0, pageSize);
    vector<string> fileStrings;

    /* Convert media files to a displayable format */
    for (const auto& file : files) {
        fileStrings.push_back(to_string(file.getIndex()) + ". " + file.getName());
    }

    /* Display media files on the view */
    mediaFileView->displayMediaFiles(fileStrings, 1, notification_message);
}

/* Function to navigate to the next page of media files */
void MediaFileController::nextPage() {
    MediaFileView* mediaFileView = dynamic_cast<MediaFileView*>(ManagerController::getInstance().getManagerView()->getView());
    if (!mediaFileView) {
        cerr << "Error: MediaFileView is null!" << endl;
        return;
    }

    auto& mediaLibrary = ManagerController::getInstance().getManagerModel()->getMediaLibrary();
    string notification_message;

    /* Check if there are more pages available */
    if (currentPage + 1 < mediaLibrary.getTotalPages(pageSize)) {
        currentPage++;
        auto files = mediaLibrary.getMediaFilesForPage(currentPage, pageSize);
        vector<string> fileStrings;

        /* Convert media files to a displayable format */
        for (const auto& file : files) {
            fileStrings.push_back(to_string(file.getIndex()) + ". " + file.getName());
        }
        notification_message = "";
        mediaFileView->displayMediaFiles(fileStrings, currentPage + 1, notification_message);

    } else {
        /* If already on the last page, show a notification */
        notification_message = "Already on the last page.";
        auto files = mediaLibrary.getMediaFilesForPage(currentPage, pageSize);
        vector<string> fileStrings;

        for (const auto& file : files) {
            fileStrings.push_back(to_string(file.getIndex()) + ". " + file.getName());
        }

        mediaFileView->displayMediaFiles(fileStrings, currentPage + 1,  notification_message);
    }
}

/* Function to navigate to the previous page of media files */
void MediaFileController::previousPage() {
    MediaFileView* mediaFileView = dynamic_cast<MediaFileView*>(ManagerController::getInstance().getManagerView()->getView());
    if (!mediaFileView) {
        cerr << "Error: MediaFileView is null!" << endl;
        return;
    }

    auto& mediaLibrary = ManagerController::getInstance().getManagerModel()->getMediaLibrary();
    string notification_message;

    /* Check if there are previous pages available */
    if (currentPage > 0) {
        currentPage--;
        auto files = mediaLibrary.getMediaFilesForPage(currentPage, pageSize);
        vector<string> fileStrings;

        /* Convert media files to a displayable format */
        for (const auto& file : files) {
            fileStrings.push_back(to_string(file.getIndex()) + ". " + file.getName());
        }
        notification_message = "";
        mediaFileView->displayMediaFiles(fileStrings, currentPage + 1, notification_message);

    } else {
        /* If already on the first page, show a notification */
        notification_message = "Already on the first page.";
        auto files = mediaLibrary.getMediaFilesForPage(currentPage, pageSize);
        vector<string> fileStrings;

        for (const auto& file : files) {
            fileStrings.push_back(to_string(file.getIndex()) + ". " + file.getName());
        }

        mediaFileView->displayMediaFiles(fileStrings,  1,   notification_message);
    }
}

/* Function to get the path of a media file by its ID */
string MediaFileController::getPathById(const vector<MediaFile>& mediaFiles, int id) {
    for (const auto& mediaFile : mediaFiles) {
        if (mediaFile.getIndex() == id) {
            return mediaFile.getPath();
        }
    }
    return "";
}

/* Function to handle user actions in the media file view */
void MediaFileController::handleAction(int action) {
    /* Check if the media library is empty */
    if (ManagerController::getInstance().getManagerModel()->getMediaLibrary().getMediaFiles().empty()) {
        clearTerminal();
        ManagerController::getInstance().getManagerView()->setView("Default");
        return;
    }

    /* Handle user actions based on the selected action */
    switch (action) {
        case ACTION_SHOW_PROPERTIES:{
            /* Show properties of a selected media file */
            MediaFileView* mediaFileView = dynamic_cast<MediaFileView*>(ManagerController::getInstance().getManagerView()->getView());
            if (!mediaFileView) {
                cerr << "Error: MediaFileView is not available!" << endl;
                break;
            }

            int mediaId = mediaFileView->getSelectedMediaID();
            if (mediaId < 1 || mediaId > static_cast<int>(ManagerController::getInstance().getManagerModel()->getMediaLibrary().getMediaFiles().size())) {
                cerr << "Error: Selected ID is invalid!" << endl;
                break;
            }

            auto& mediaFiles = ManagerController::getInstance().getManagerModel()->getMediaLibrary().getMediaFiles();
            string filepath = getPathById(mediaFiles, mediaId);

            ManagerController::getInstance().getManagerView()->setView("Metadata");

            /* Display metadata of the selected media file */
            MetadataController* metadataController = dynamic_cast<MetadataController*>(ManagerController::getInstance().getController("Metadata"));
            if (!metadataController) {
                cerr << "Error: MetadataController is not available!" << endl;
                break;
            }

            clearTerminal();
            metadataController->handleShowMetadata(filepath);
            ManagerController::getInstance().getManagerView()->setView("Metadata");

            break;
        }
        case ACTION_NEXT_PAGE:
            /* Navigate to the next page of media files */
            clearTerminal();
            nextPage();
            break;
        case ACTION_PREVIOUS_PAGE:
            /* Navigate to the previous page of media files */
            clearTerminal();
            previousPage();
            break;
        case ACTION_PLAY_MEDIA: {
            /* Play the selected media file */
            MediaFileView* mediaFileView = dynamic_cast<MediaFileView*>(ManagerController::getInstance().getManagerView()->getView());
            if (!mediaFileView) {
                cerr << "Error: MediaFileView is not available!" << endl;
                break;
            }

            int mediaId = mediaFileView->getSelectedMediaID();

            /* Retrieve the selected media file */
            auto& mediaFiles = ManagerController::getInstance().getManagerModel()->getMediaLibrary().getMediaFiles();
            MediaFile* selectedMedia = nullptr;

            for (auto& file : mediaFiles) {
                if (file.getIndex() == mediaId) {
                    selectedMedia = &file;
                    break;
                }
            }

            if (selectedMedia) {
                /* Play the selected media file */
                PlayingMediaController* playingController = dynamic_cast<PlayingMediaController*>(
                    ManagerController::getInstance().getController("PlayingView"));
                if (!playingController) {
                    cerr << "Error: PlayingMediaController is not available!\n";
                    break;
                }

                ManagerController::getInstance().getManagerView()->setView("PlayingView");
                playingController->playMediaFile(selectedMedia);

            } else {
                cerr << "Error: Media file not found!\n";
            }
            break;
        }
        case ACTION_ADD_TO_PLAYLIST: {
            /* Add the selected media file to a playlist */
            MediaFileView* mediaFileView = dynamic_cast<MediaFileView*>(ManagerController::getInstance().getManagerView()->getView());
            if (!mediaFileView) {
                cerr << "Error: MediaFileView is not available!" << endl;
                break;
            }

            int mediaId = mediaFileView->getSelectedMediaID();

            /* Retrieve the selected media file and available playlists */
            auto& mediaLibrary = ManagerModel::getInstance().getMediaLibrary();
            const auto& mediaFiles = mediaLibrary.getMediaFiles();

            auto it = find_if(mediaFiles.begin(), mediaFiles.end(),
                [mediaId](const MediaFile& file) { return file.getIndex() == mediaId; });

            PlaylistLibrary& playlistLibrary = ManagerModel::getInstance().getPlaylistLibrary();
            auto& playlists = playlistLibrary.getPlaylists();

            if (playlists.empty()) {
                cout << "No playlists available.\n";
                break;
            }

            /* Display playlists for the user to select */
            ManagerController::getInstance().getManagerView()->setView("Playlist");
            PlaylistView* playlistView = dynamic_cast<PlaylistView*>(ManagerController::getInstance().getManagerView()->getView());
            if (!playlistView) {
                cerr << "Error: PlaylistView is not available!\n";
                break;
            }

            playlistView->displayPlaylists(playlists);

            int selected_playlist_ID = playlistView->getSelectedPlaylistID();
            Playlist& selectedPlaylist = playlists[selected_playlist_ID - 1];

            /* Add the media file to the selected playlist */
            selectedPlaylist.addSong(*it);

            string notification_message = "Media file '" + it->getName() + "' added to playlist '" + selectedPlaylist.getName() + "'.";
            bool success = true;

            try {
                /* Save the updated playlist to file */
                playlistLibrary.saveToFile("playlists.txt");
                notification_message += "\nUpdated playlist saved to file successfully.";
            } catch (const exception& e) {
                notification_message = "Error saving playlist to file: " + string(e.what());
                success = false;
            }

            /* Display a notification to the user */
            auto screen = ScreenInteractive::TerminalOutput();

            auto notification_renderer = Renderer([&] {
                return vbox({
                    text("Media File View") | bold | center,
                    separator(),
                    text(notification_message) | (success ? color(Color::Green) : color(Color::Red)) | center,
                    separator(),
                    text("Press ENTER to return to Media File View.") | dim | center
                });
            });

            auto main_component = CatchEvent(notification_renderer, [&](Event event) {
                if (event == Event::Return) {
                    screen.ExitLoopClosure()();
                    return true;
                }
                return false;
            });

            screen.Loop(main_component);
            clearTerminal();

            /* Return to the media file view */
            MediaFileController* mediaFileController = dynamic_cast<MediaFileController*>(
                ManagerController::getInstance().getController("MediaFile"));

            if (!mediaFileController) {
                cerr << "Error: MediaFileController is not available!" << endl;
                break;
            }
            ManagerController::getInstance().getManagerView()->setView("MediaFile");
            mediaFileController->scanAndDisplayMedia();

            break;
        }
        case ACTION_RETURN_TO_PLAYING: {
            /* Return to the currently playing media view */
            ManagerController::getInstance().getManagerView()->setView("PlayingView");
            PlayingView* playingView = dynamic_cast<PlayingView*>(
                ManagerController::getInstance().getManagerView()->getView());
            playingView -> showMenu();
            break;
            }
        case ACTION_RETURN_HOME:{
            /* Stop the current media and return to the home screen */
            PlayingMediaController* playingController = dynamic_cast<PlayingMediaController*>(
                ManagerController::getInstance().getController("PlayingView"));

            if (!playingController) {
                cerr << "Error: PlayingMediaController not available!\n";
                break;
            }
            playingController->stop();
            clearTerminal();
            ManagerController::getInstance().getManagerView()->setView("Default");
            break;
        }
        default:
            cout << "Invalid choice! Please try again. " << endl;
            break;
    }
}
