
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
    try {
        ManagerController::getInstance().getManagerModel()->getMediaLibrary().scanDirectory(path);
        cout << "Directory scanned successfully.\n";
    } catch (const exception& e) {
        cerr << "Error scanning directory: " << e.what() << endl;
    }
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
            string usbRootPath = "/media/kist";  
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
    int totalPages = mediaLibrary.getTotalPages(pageSize);

    
    auto files = mediaLibrary.getMediaFilesForPage(0, pageSize);
    vector<string> fileStrings;

    for (const auto& file : files) {
        fileStrings.push_back(to_string(file.getIndex()) + ". " + file.getName());
    }

    
    mediaFileView->displayMediaFiles(fileStrings, 1);
    mediaFileView->displayPagination(1, totalPages);
}


void MediaFileController::nextPage() {
    MediaFileView* mediaFileView = dynamic_cast<MediaFileView*>(ManagerController::getInstance().getManagerView()->getView());
    if (!mediaFileView) {
        cerr << "Error: MediaFileView is null!" << endl;
        return;
    }

    
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
        auto files = mediaLibrary.getMediaFilesForPage(currentPage, pageSize);
        vector<string> fileStrings;

        for (const auto& file : files) {
            fileStrings.push_back(to_string(file.getIndex()) + ". " + file.getName());
        }

        mediaFileView->displayMediaFiles(fileStrings, currentPage + 1);
        mediaFileView->displayPagination(currentPage + 1, mediaLibrary.getTotalPages(pageSize));
    }
}


void MediaFileController::previousPage() {
    MediaFileView* mediaFileView = dynamic_cast<MediaFileView*>(ManagerController::getInstance().getManagerView()->getView());
    if (!mediaFileView) {
        cerr << "Error: MediaFileView is null!" << endl;
        return;
    }

    
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
        auto files = mediaLibrary.getMediaFilesForPage(currentPage, pageSize);
        vector<string> fileStrings;

        for (const auto& file : files) {
            fileStrings.push_back(to_string(file.getIndex()) + ". " + file.getName());
        }

        mediaFileView->displayMediaFiles(fileStrings,  1);
        mediaFileView->displayPagination(1, mediaLibrary.getTotalPages(pageSize));
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
        ManagerController::getInstance().getManagerView()->setView("Default");
        return; 
    }
    switch (action) {
        case ACTION_SHOW_PROPERTIES:{
            int mediaId;
            do
            {
                cout << "\nEnter Media ID to show properties: ";
                cin >> mediaId;
                if(mediaId < 1 || mediaId > static_cast<int>(ManagerController::getInstance().getManagerModel()->getMediaLibrary().getMediaFiles().size()))
                {
                    cout << "\nInvalid ID, please input ID from 1 to " << static_cast<int>(ManagerController::getInstance().getManagerModel()->getMediaLibrary().getMediaFiles().size()) << "!";
                    cin.ignore(INT_MAX, '\n');
                }
            } while (mediaId < 1 || mediaId > static_cast<int>(ManagerController::getInstance().getManagerModel()->getMediaLibrary().getMediaFiles().size()));

            
            auto& mediaFiles = ManagerController::getInstance().getManagerModel()->getMediaLibrary().getMediaFiles();
            string filepath = getPathById(mediaFiles, mediaId);

            
            ManagerController::getInstance().getManagerView()->setView("Metadata");

            MetadataController* metadataController = dynamic_cast<MetadataController*>(ManagerController::getInstance().getController("Metadata"));
            if (!metadataController) {
                cerr << "Error: MetadataController is not available!" << endl;
                break;
            }

            
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
        case ACTION_PLAY_MEDIA: {
            int mediaId;
            do
            {
                cout << "\nEnter Media ID to Play: ";
                cin >> mediaId;
                if(mediaId < 1 || mediaId > static_cast<int>(ManagerController::getInstance().getManagerModel()->getMediaLibrary().getMediaFiles().size()))
                {
                    cout << "\nInvalid ID, please input ID from 1 to " << static_cast<int>(ManagerController::getInstance().getManagerModel()->getMediaLibrary().getMediaFiles().size()) << "!";
                    cin.ignore(INT_MAX, '\n');
                }
            } while (mediaId < 1 || mediaId > static_cast<int>(ManagerController::getInstance().getManagerModel()->getMediaLibrary().getMediaFiles().size()));

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
            int mediaId;
            do
            {
                cout << "\nEnter Media ID to add a playlist: ";
                cin >> mediaId;
                if(mediaId < 1 || mediaId > static_cast<int>(ManagerController::getInstance().getManagerModel()->getMediaLibrary().getMediaFiles().size()))
                {
                    cout << "\nInvalid ID, please input ID from 1 to " << static_cast<int>(ManagerController::getInstance().getManagerModel()->getMediaLibrary().getMediaFiles().size()) << "!";
                    cin.ignore(INT_MAX, '\n');
                }
            } while (mediaId < 1 || mediaId > static_cast<int>(ManagerController::getInstance().getManagerModel()->getMediaLibrary().getMediaFiles().size()));

            
            auto& mediaLibrary = ManagerModel::getInstance().getMediaLibrary();
            const auto& mediaFiles = mediaLibrary.getMediaFiles();

            
            auto it = find_if(mediaFiles.begin(), mediaFiles.end(),
                [mediaId](const MediaFile& file) { return file.getIndex() == mediaId; });

            if (it == mediaFiles.end()) {
                cerr << "Error: Invalid Media ID!\n";
                break;
            }

            
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

            
            int playlistId;
            
            
            do
            {
                cout << "\nEnter Media ID to add a playlist: ";
                cin >> playlistId;
                if(playlistId < 1 || playlistId > static_cast<int>(playlists.size()))
                {
                    cout << "\nInvalid ID, please input ID from 1 to " << static_cast<int>(playlists.size()) << "!";
                    cin.ignore(INT_MAX, '\n');
                }
            } while (playlistId < 1 || playlistId > static_cast<int>(playlists.size()));

            
            if (playlistId <= 0 || playlistId > static_cast<int>(playlists.size())) {
                cerr << "Error: Invalid Playlist ID!\n";
                break;
            }

            
            Playlist& selectedPlaylist = playlists[playlistId - 1];

            
            selectedPlaylist.addSong(*it);
            cout << "Media file '" << it->getName() << "' added to playlist '" << selectedPlaylist.getName() << "'.\n";

            
            try {
                playlistLibrary.saveToFile("playlists.txt");
                cout << "Updated playlist saved to file successfully.\n";
            } catch (const exception& e) {
                cerr << "Error saving playlist to file: " << e.what() << '\n';
            }

            
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
        case ACTION_RETURN_TO_PLAYING: {
     PlayingMediaController* playingController = dynamic_cast<PlayingMediaController*>(
        ManagerController::getInstance().getController("PlayingView"));

    if (!playingController) {
        cerr << "Error: PlayingMediaController not available!\n";
        break;
    }

    MediaFile* currentFile = playingController->getCurrentMediaFile();
    if (currentFile) {
        
        ManagerController::getInstance().getManagerView()->setView("PlayingView");
        cout << "Returning to Now Playing screen...\n";

        
        PlayingView* playingView = dynamic_cast<PlayingView*>(
            ManagerController::getInstance().getManagerView()->getView());
        if (playingView) {
            playingView->clearView();
            playingView->displayPlayingView(
                currentFile->getName(),
                playingController->getTotalTime(),
                playingController->getVolume(),
                playingController->getCurrentTime()
            );
        }

        
        playingController->startDisplayLoop();
    } else {
        cerr << "No song is currently selected.\n";
    }
    break;
        }
        case ACTION_RETURN_HOME:{
        cout << "\nReturning Home...\n";
        PlayingMediaController* playingController = dynamic_cast<PlayingMediaController*>(
            ManagerController::getInstance().getController("PlayingView"));

        if (!playingController) {
            cerr << "Error: PlayingMediaController not available!\n";
            break;}
        playingController->stop();
        ManagerController::getInstance().getManagerView()->setView("Default");
            
            break;
        }
        default:
            cout << "Invalid choice! Please try again. " << endl;
            break;
    }
}

