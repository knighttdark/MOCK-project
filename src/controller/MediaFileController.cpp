
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

    // Hiển thị thông báo bằng FXUI
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
            string usbRootPath;

            // Tự động lấy username hiện tại để tạo đường dẫn USB root path
            try {
                char* username = getenv("USER"); // Lấy tên user từ biến môi trường
                if (username == nullptr) {
                    cerr << "Error: Could not retrieve current username." << endl;
                    return;
                }

                usbRootPath = "/media/" + string(username); // Đường dẫn /media/<username>
            } catch (const exception& e) {
                cerr << "Error retrieving USB root path: " << e.what() << endl;
                return;
            }

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
    string notification_message = "";
    int totalPages = mediaLibrary.getTotalPages(pageSize);

    /* Retrieve the media files for the first page */
    auto files = mediaLibrary.getMediaFilesForPage(0, pageSize);
    vector<string> fileStrings;

    for (const auto& file : files) {
        fileStrings.push_back(to_string(file.getIndex()) + ". " + file.getName());
    }

    /* Display the first page of media files */
    mediaFileView->displayMediaFiles(fileStrings, 1, notification_message);
    
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

/* Displays the previous page of media files */
void MediaFileController::previousPage() {
    MediaFileView* mediaFileView = dynamic_cast<MediaFileView*>(ManagerController::getInstance().getManagerView()->getView());
    if (!mediaFileView) {
        cerr << "Error: MediaFileView is null!" << endl;
        return;
    }

    /* Access the media library and go to the previous page if available */
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

            MediaFileView* mediaFileView = dynamic_cast<MediaFileView*>(ManagerController::getInstance().getManagerView()->getView());
            if (!mediaFileView) {
                cerr << "Error: MediaFileView is not available!" << endl;
                break;
            }

            // Get the selected media ID
            int mediaId = mediaFileView->getSelectedMediaID();
            if (mediaId < 1 || mediaId > static_cast<int>(ManagerController::getInstance().getManagerModel()->getMediaLibrary().getMediaFiles().size())) {
                cerr << "Error: Selected ID is invalid!" << endl;
                break;
            }
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
            MediaFileView* mediaFileView = dynamic_cast<MediaFileView*>(ManagerController::getInstance().getManagerView()->getView());
            if (!mediaFileView) {
                cerr << "Error: MediaFileView is not available!" << endl;
                break;
            }

            // Get the selected media ID
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
                // Switch to PlayingView and play the selected media
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

            /* Get the selected media ID */
            int mediaId = mediaFileView->getSelectedMediaID();

            /* Get the list of media files */
            auto& mediaLibrary = ManagerModel::getInstance().getMediaLibrary();
            const auto& mediaFiles = mediaLibrary.getMediaFiles();

            /* Find the media file by ID */
            auto it = find_if(mediaFiles.begin(), mediaFiles.end(),
                [mediaId](const MediaFile& file) { return file.getIndex() == mediaId; });

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

            // Hiển thị danh sách playlists và chờ người dùng chọn
            playlistView->displayPlaylists(playlists);

            /* Lấy ID của playlist được chọn */
            int selected_playlist_ID = playlistView->getSelectedPlaylistID();

            /* Lấy playlist được chọn */
            Playlist& selectedPlaylist = playlists[selected_playlist_ID - 1];

            /* Thêm media vào playlist */
            selectedPlaylist.addSong(*it);

            // Thông báo thành công
            std::string notification_message = "Media file '" + it->getName() + "' added to playlist '" + selectedPlaylist.getName() + "'.";
            bool success = true;

            try {
                /* Save the updated playlist to the file */
                playlistLibrary.saveToFile("playlists.txt");
                notification_message += "\nUpdated playlist saved to file successfully.";
            } catch (const std::exception& e) {
                notification_message = "Error saving playlist to file: " + std::string(e.what());
                success = false; // Đặt cờ lỗi
            }

            // Tạo giao diện hiển thị thông báo
            auto screen = ScreenInteractive::TerminalOutput();

            auto notification_renderer = Renderer([&] {
                return vbox({
                    text("Media File View") | bold | center, // Tiêu đề chính
                    separator(),
                    text(notification_message) | (success ? color(Color::Green) : color(Color::Red)) | center, // Hiển thị thông báo
                    separator(),
                    text("Press ENTER to return to Media File View.") | dim | center // Hướng dẫn người dùng
                });
            });

            // Xử lý sự kiện nhấn ENTER để quay lại MediaFileView
            auto main_component = CatchEvent(notification_renderer, [&](Event event) {
                if (event == Event::Return) {
                    screen.ExitLoopClosure()(); // Thoát màn hình
                    return true;
                }
                return false;
            });

            // Hiển thị giao diện thông báo
            screen.Loop(main_component);

            /* Return MediaFile */
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
        PlayingMediaController* playingController = dynamic_cast<PlayingMediaController*>(
            ManagerController::getInstance().getController("PlayingView"));

        if (!playingController) {
            std::cerr << "Error: PlayingMediaController not available!\n";
            break;
        }

        MediaFile* currentFile = playingController->getCurrentMediaFile();
        if (currentFile) {
            // Chuyển giao diện về PlayingView
            ManagerController::getInstance().getManagerView()->setView("PlayingView");
            std::cout << "Returning to Now Playing screen...\n";

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
            std::cerr << "No song is currently selected.\n";
        }
        break;
            }
        case ACTION_RETURN_HOME:{
        cout << "\nReturning Home...\n";
        PlayingMediaController* playingController = dynamic_cast<PlayingMediaController*>(
            ManagerController::getInstance().getController("PlayingView"));

        if (!playingController) {
            std::cerr << "Error: PlayingMediaController not available!\n";
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

