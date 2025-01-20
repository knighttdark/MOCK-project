
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
            string usbRootPath = "/media/kist";  // Đường dẫn tới USB root path
            vector<string> usbPaths;

            // Duyệt qua thư mục USB để tìm các thiết bị USB kết nối
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

            // Nếu không có thiết bị USB nào
            if (usbPaths.empty()) {
                cout << "No USB devices found in " << usbRootPath << endl;
                return;
            }

            // Sử dụng FTXUI để hiển thị danh sách USB
            int selected = 0;
            string error_message;
            vector<string> usbEntries;
            for (size_t i = 0; i < usbPaths.size(); ++i) {
                usbEntries.push_back(to_string(i + 1) + ". " + usbPaths[i]);
            }

            auto screen = ScreenInteractive::TerminalOutput();
            auto menu = Menu(&usbEntries, &selected);

            // Tạo giao diện
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

            // Xử lý sự kiện
            screen.Loop(renderer | CatchEvent([&](Event event) {
                // Khi nhấn Enter
                if (event == Event::Return) {
                    if (selected >= 0 && selected < usbPaths.size()) {
                        // Quét USB đã chọn
                        string selectedUsbPath = usbPaths[selected];
                        cout << "Scanning USB: " << selectedUsbPath << endl;
                        scanDirectory(selectedUsbPath);  // Gọi hàm scan directory
                    }
                    screen.ExitLoopClosure()();
                    return true;
                }

                // Xử lý sự kiện click chuột
                if (event.is_mouse() && event.mouse().button == Mouse::Left) {
                    int clicked_index = event.mouse().y - 3; // Điều chỉnh theo vị trí hiển thị trong giao diện
                    if (clicked_index >= 0 && clicked_index < usbEntries.size()) {
                        selected = clicked_index;  // Cập nhật lựa chọn theo chỉ số người dùng click
                        string selectedUsbPath = usbPaths[selected];
                        cout << "Scanning USB: " << selectedUsbPath << endl;
                        scanDirectory(selectedUsbPath);  // Gọi hàm scan directory
                        screen.ExitLoopClosure()();  // Thoát giao diện
                    }
                    return true;
                }

                return false; // Không xử lý sự kiện nào khác
            }));
            break;
        }
        

        case RETURN_HOME:{            
            /* Return to the main menu */
            ManagerController::getInstance().getManagerView()->setView("Default");
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
        auto files = mediaLibrary.getMediaFilesForPage(currentPage, pageSize);
        vector<string> fileStrings;

        for (const auto& file : files) {
            fileStrings.push_back(to_string(file.getIndex()) + ". " + file.getName());
        }

        mediaFileView->displayMediaFiles(fileStrings, currentPage + 1);
        mediaFileView->displayPagination(currentPage + 1, mediaLibrary.getTotalPages(pageSize));
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
        auto files = mediaLibrary.getMediaFilesForPage(currentPage, pageSize);
        vector<string> fileStrings;

        for (const auto& file : files) {
            fileStrings.push_back(to_string(file.getIndex()) + ". " + file.getName());
        }

        mediaFileView->displayMediaFiles(fileStrings,  1);
        mediaFileView->displayPagination(1, mediaLibrary.getTotalPages(pageSize));
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
    if (ManagerController::getInstance().getManagerModel()->getMediaLibrary().getMediaFiles().empty()) {
        ManagerController::getInstance().getManagerView()->setView("Default");
        return; // Thoát khỏi hàm
    }
    switch (action) {
        case ACTION_SHOW_PROPERTIES:{
            int mediaId;
            // cout << "\nEnter Media ID to show properties: ";
            // cin >> mediaId;
            // cin.ignore(INT_MAX, '\n');
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
        case ACTION_PLAY_MEDIA: {            /* Play media file */
            int mediaId;
            // cout << "\nEnter Media ID to Play: ";
            // cin >> mediaId;
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
                // Switch to PlayingView and play the selected media
                ManagerController::getInstance().getManagerView()->setView("PlayingView");
                playingController->playMediaFile(selectedMedia);

            } else {
                cerr << "Error: Media file not found!\n";
        }
        break;
        }
        case ACTION_ADD_TO_PLAYLIST: {
            /* Add media file to playlist */
            int mediaId;
            // cout << "\nEnter Media ID to add to a playlist: ";
            // cin >> mediaId;
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
            // cout << "\nEnter Playlist ID to add media '" << it->getName() << "': ";
            // cin >> playlistId;
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
        case ACTION_RETURN_TO_PLAYING: {
     PlayingMediaController* playingController = dynamic_cast<PlayingMediaController*>(
        ManagerController::getInstance().getController("PlayingView"));

    if (!playingController) {
        cerr << "Error: PlayingMediaController not available!\n";
        break;
    }

    MediaFile* currentFile = playingController->getCurrentMediaFile();
    if (currentFile) {
        // Chuyển giao diện về PlayingView
        ManagerController::getInstance().getManagerView()->setView("PlayingView");
        cout << "Returning to Now Playing screen...\n";

        // Hiển thị lại thông tin ngay lập tức
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

        // Tiếp tục vòng lặp cập nhật
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
            //system("clear");
            break;
        }
        default:
            cout << "Invalid choice! Please try again. " << endl;
            break;
        
    }
}

