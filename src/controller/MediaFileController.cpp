
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

MediaFileController::MediaFileController(){}

void MediaFileController::scanDirectory(const string& path) {
    std::string notification_message;
    bool success = false;

    try {
        ManagerController::getInstance().getManagerModel()->getMediaLibrary().scanDirectory(path);
        notification_message = "Directory '" + path + "' scanned successfully.";
        success = true;
    } catch (const exception& e) {
        notification_message = "Error scanning directory: " + std::string(e.what());
        success = false;
    }

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
            string directoryPath = mediaFileView->promptDirectoryInput();
            scanDirectory(directoryPath);
            break;
        }
        case SCAN_USB: {
            string usbRootPath;

            try {
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
            ManagerController::getInstance().getManagerView()->setView("Default");
            break;
        }
        default:
            cerr << "Invalid scan option!" << endl;
            break;
    }
}

void MediaFileController::scanAndDisplayMedia() {
    MediaFileView* mediaFileView = dynamic_cast<MediaFileView*>(ManagerController::getInstance().getManagerView()->getView());
    if (!mediaFileView) {
        cerr << "Error: MediaFileView is null!" << endl;
        return;
    }

    auto& mediaLibrary = ManagerController::getInstance().getManagerModel()->getMediaLibrary();
    string notification_message = "";

    auto files = mediaLibrary.getMediaFilesForPage(0, pageSize);
    vector<string> fileStrings;

    for (const auto& file : files) {
        fileStrings.push_back(to_string(file.getIndex()) + ". " + file.getName());
    }

    mediaFileView->displayMediaFiles(fileStrings, 1, notification_message);
}


void MediaFileController::nextPage() {
    MediaFileView* mediaFileView = dynamic_cast<MediaFileView*>(ManagerController::getInstance().getManagerView()->getView());
    if (!mediaFileView) {
        cerr << "Error: MediaFileView is null!" << endl;
        return;
    }

    auto& mediaLibrary = ManagerController::getInstance().getManagerModel()->getMediaLibrary();
    string notification_message;

    if (currentPage + 1 < mediaLibrary.getTotalPages(pageSize)) {
        currentPage++;
        auto files = mediaLibrary.getMediaFilesForPage(currentPage, pageSize);
        vector<string> fileStrings;

        for (const auto& file : files) {
            fileStrings.push_back(to_string(file.getIndex()) + ". " + file.getName());
        }
        notification_message = "";
        mediaFileView->displayMediaFiles(fileStrings, currentPage + 1, notification_message);

    } else {
        notification_message = "Already on the last page.";
        auto files = mediaLibrary.getMediaFilesForPage(currentPage, pageSize);
        vector<string> fileStrings;

        for (const auto& file : files) {
            fileStrings.push_back(to_string(file.getIndex()) + ". " + file.getName());
        }

        mediaFileView->displayMediaFiles(fileStrings, currentPage + 1,  notification_message);
    }
}


void MediaFileController::previousPage() {
    MediaFileView* mediaFileView = dynamic_cast<MediaFileView*>(ManagerController::getInstance().getManagerView()->getView());
    if (!mediaFileView) {
        cerr << "Error: MediaFileView is null!" << endl;
        return;
    }

    auto& mediaLibrary = ManagerController::getInstance().getManagerModel()->getMediaLibrary();
    string notification_message;

    if (currentPage > 0) {
        currentPage--;
        auto files = mediaLibrary.getMediaFilesForPage(currentPage, pageSize);
        vector<string> fileStrings;

        for (const auto& file : files) {
            fileStrings.push_back(to_string(file.getIndex()) + ". " + file.getName());
        }
        notification_message = "";
        mediaFileView->displayMediaFiles(fileStrings, currentPage + 1, notification_message);

    } else {
        notification_message = "Already on the first page.";
        auto files = mediaLibrary.getMediaFilesForPage(currentPage, pageSize);
        vector<string> fileStrings;

        for (const auto& file : files) {
            fileStrings.push_back(to_string(file.getIndex()) + ". " + file.getName());
        }

        mediaFileView->displayMediaFiles(fileStrings,  1,   notification_message);
    }
}


string MediaFileController::getPathById(const vector<MediaFile>& mediaFiles, int id) {
    for (const auto& mediaFile : mediaFiles) {
        if (mediaFile.getIndex() == id) {
            return mediaFile.getPath();
        }
    }
    return "";
}

void MediaFileController::handleAction(int action) {
    if (ManagerController::getInstance().getManagerModel()->getMediaLibrary().getMediaFiles().empty()) {
        clearTerminal();
        ManagerController::getInstance().getManagerView()->setView("Default");
        return;
    }
    switch (action) {
        case ACTION_SHOW_PROPERTIES:{

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
            clearTerminal();
            nextPage();
            break;
        case ACTION_PREVIOUS_PAGE:
            clearTerminal();
            previousPage();
            break;
        case ACTION_PLAY_MEDIA: {
            MediaFileView* mediaFileView = dynamic_cast<MediaFileView*>(ManagerController::getInstance().getManagerView()->getView());
            if (!mediaFileView) {
                cerr << "Error: MediaFileView is not available!" << endl;
                break;
            }

            int mediaId = mediaFileView->getSelectedMediaID();

            auto& mediaFiles = ManagerController::getInstance().getManagerModel()->getMediaLibrary().getMediaFiles();
            MediaFile* selectedMedia = nullptr;

            for (auto& file : mediaFiles) {
                if (file.getIndex() == mediaId) {
                    selectedMedia = &file;
                    break;
                }
            }

            if (selectedMedia) {
                PlayingMediaController* playingController = dynamic_cast<PlayingMediaController*>(
                    ManagerController::getInstance().getController("PlayingView"));
                if (!playingController) {
                    std::cerr << "Error: PlayingMediaController is not available!\n";
                    break;
                }

                ManagerController::getInstance().getManagerView()->setView("PlayingView");
                playingController->playMediaFile(selectedMedia);

            } else {
                std::cerr << "Error: Media file not found!\n";
        }
        break;
        }

        case ACTION_ADD_TO_PLAYLIST: {
        MediaFileView* mediaFileView = dynamic_cast<MediaFileView*>(ManagerController::getInstance().getManagerView()->getView());
            if (!mediaFileView) {
                cerr << "Error: MediaFileView is not available!" << endl;
                break;
            }

            int mediaId = mediaFileView->getSelectedMediaID();

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

            ManagerController::getInstance().getManagerView()->setView("Playlist");
            PlaylistView* playlistView = dynamic_cast<PlaylistView*>(ManagerController::getInstance().getManagerView()->getView());
            if (!playlistView) {
                cerr << "Error: PlaylistView is not available!\n";
                break;
            }

            playlistView->displayPlaylists(playlists);

            int selected_playlist_ID = playlistView->getSelectedPlaylistID();

            Playlist& selectedPlaylist = playlists[selected_playlist_ID - 1];

            selectedPlaylist.addSong(*it);

            std::string notification_message = "Media file '" + it->getName() + "' added to playlist '" + selectedPlaylist.getName() + "'.";
            bool success = true;

            try {
                playlistLibrary.saveToFile("playlists.txt");
                notification_message += "\nUpdated playlist saved to file successfully.";
            } catch (const std::exception& e) {
                notification_message = "Error saving playlist to file: " + std::string(e.what());
                success = false;
            }

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
            ManagerController::getInstance().getManagerView()->setView("PlayingView");
            PlayingView* playingView = dynamic_cast<PlayingView*>(
                ManagerController::getInstance().getManagerView()->getView());
            playingView -> showMenu();
        break;
            }
        case ACTION_RETURN_HOME:{
        PlayingMediaController* playingController = dynamic_cast<PlayingMediaController*>(
            ManagerController::getInstance().getController("PlayingView"));

        if (!playingController) {
            std::cerr << "Error: PlayingMediaController not available!\n";
            break;}
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
