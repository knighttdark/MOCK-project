
#include "controller/MediaFileController.h"
#include "controller/ManagerController.h"
#include "model/PlaylistLibrary.h"
#include <stdexcept>
#include <iostream>
#include <climits>

MediaFileController::MediaFileController(){}


void MediaFileController::scanDirectory(const std::string& path) {
    try {
        ManagerController::getInstance().getManagerModel()->getMediaLibrary().scanDirectory(path);
        std::cout << "Directory scanned successfully.\n";
    } catch (const std::exception& e) {
        std::cerr << "Error scanning directory: " << e.what() << std::endl;
    }
}


void MediaFileController::nextPage() {
    MediaFileView* mediaFileView = dynamic_cast<MediaFileView*>(ManagerController::getInstance().getManagerView()->getView());
    if (!mediaFileView) {
        std::cerr << "Error: MediaFileView is null!" << std::endl;
        return;
    }

    auto& mediaLibrary =  ManagerController::getInstance().getManagerModel()->getMediaLibrary();
    if (currentPage + 1 < mediaLibrary.getTotalPages(pageSize)) {
        currentPage++;
        auto files = mediaLibrary.getMediaFilesForPage(currentPage, pageSize);
        std::vector<std::string> fileStrings;
        for (const auto& file : files) {
            fileStrings.push_back(std::to_string(file.getIndex()) + ". " + file.getName());
        }

        mediaFileView->displayMediaFiles(fileStrings, currentPage + 1);
        mediaFileView->displayPagination(currentPage + 1, mediaLibrary.getTotalPages(pageSize));
    } else {
        std::cout << "Already on the last page.\n";
    }
}

void MediaFileController::previousPage() {
    MediaFileView* mediaFileView = dynamic_cast<MediaFileView*>(ManagerController::getInstance().getManagerView()->getView());
    if (!mediaFileView) {
        std::cerr << "Error: MediaFileView is null!" << std::endl;
        return;
    }

    auto& mediaLibrary = ManagerController::getInstance().getManagerModel()->getMediaLibrary();
    if (currentPage > 0) {
        currentPage--;
        auto files = mediaLibrary.getMediaFilesForPage(currentPage, pageSize);
        std::vector<std::string> fileStrings;
        for (const auto& file : files) {
            fileStrings.push_back(std::to_string(file.getIndex()) + ". " + file.getName());
        }

        mediaFileView->displayMediaFiles(fileStrings, currentPage + 1);
        mediaFileView->displayPagination(currentPage + 1, mediaLibrary.getTotalPages(pageSize));
    } else {
        std::cout << "Already on the first page.\n";
    }
}


void MediaFileController::scanAndDisplayMedia() {
    MediaFileView* mediaFileView = dynamic_cast<MediaFileView*>(ManagerController::getInstance().getManagerView()->getView());
    if (!mediaFileView) {
        std::cerr << "Error: MediaFileView is null!" << std::endl;
        return;
    }

    // Access media library and initialize pagination
    auto& mediaLibrary = ManagerController::getInstance().getManagerModel()->getMediaLibrary();
    int totalPages = mediaLibrary.getTotalPages(pageSize);

    // Get MediaFile objects and convert to strings for display
    auto files = mediaLibrary.getMediaFilesForPage(0, pageSize);
    std::vector<std::string> fileStrings;

    for (const auto& file : files) {
    fileStrings.push_back(std::to_string(file.getIndex()) + ". " + file.getName());
    }

    // Display first page of media files
    mediaFileView->displayMediaFiles(fileStrings, 1);
    mediaFileView->displayPagination(1, mediaLibrary.getTotalPages(pageSize));
    // Exit after displaying the first page
    return;
}

std::string MediaFileController::getPathById(const std::vector<MediaFile>& mediaFiles, int id) {
    for (const auto& mediaFile : mediaFiles) {
        if (mediaFile.getIndex() == id) { 
            return mediaFile.getPath();   
        }
    }
    return ""; 
}

void MediaFileController::handleAction(int action) {
    switch (action) {
    case 1:{
        int mediaId;
        std::cout << "\nEnter Media ID to show properties: ";
        std::cin >> mediaId;
        std::cin.ignore(INT_MAX, '\n');
        //getPath 
        auto& mediaFiles = ManagerController::getInstance().getManagerModel()->getMediaLibrary().getMediaFiles();
        std::string filepath = getPathById(mediaFiles, mediaId);
    
         // Chuyển View sang MetadataView
        ManagerController::getInstance().getManagerView()->setView("Metadata");

        MetadataController* metadataController = dynamic_cast<MetadataController*>(ManagerController::getInstance().getController("Metadata"));
        if (!metadataController) {
            std::cerr << "Error: MetadataController is not available!" << std::endl;
            break;
        }
        
        // Gọi logic hiển thị metadata từ MetadataController
        std::cout << "\nShowing Metadata..." << std::endl;
        metadataController->handleShowMetadata(filepath);
        ManagerController::getInstance().getManagerView()->setView("Metadata");
        
        break;
    }
    case 2:
        std::cout << "\nEditing Metadata..." << std::endl;
        //mediaFileView->editMetadata(managerModel->getMediaLibrary().getMetadata());
        break;
    case 3:
        std::cout << "\nGoing to Next Page..." << std::endl;
        nextPage();
        break;
    case 4:
        std::cout << "\nGoing to Previous Page..." << std::endl;
        previousPage();
        break;
    case 5:
        int mediaId;
        std::cout << "\nEnter Media ID to Play: ";
        std::cin >> mediaId;
        std::cout << "\nPlaying Media with ID: " << mediaId << std::endl;
        //model->getMediaLibrary().playMedia(mediaId);
        break;

        case 6: {
    // Yêu cầu người dùng nhập ID bài hát
    int mediaId;
    std::cout << "\nEnter Media ID to add to a playlist: ";
    std::cin >> mediaId;

    // Lấy danh sách tệp phương tiện
    auto& mediaLibrary = ManagerModel::getInstance().getMediaLibrary();
    const auto& mediaFiles = mediaLibrary.getMediaFiles();

    // Tìm tệp phương tiện theo ID
    auto it = std::find_if(mediaFiles.begin(), mediaFiles.end(),
                           [mediaId](const MediaFile& file) { return file.getIndex() == mediaId; });

    if (it == mediaFiles.end()) {
        std::cerr << "Error: Invalid Media ID!\n";
        break;
    }

    // Lấy danh sách playlist từ PlaylistLibrary
    PlaylistLibrary& playlistLibrary = ManagerModel::getInstance().getPlaylistLibrary();
    auto& playlists = playlistLibrary.getPlaylists(); // Lấy tham chiếu

    // Kiểm tra nếu danh sách playlist trống
    if (playlists.empty()) {
        std::cout << "No playlists available.\n";
        break;
    }

    // Hiển thị danh sách playlist
    ManagerController::getInstance().getManagerView()->setView("Playlist");
    PlaylistView* playlistView = dynamic_cast<PlaylistView*>(ManagerController::getInstance().getManagerView()->getView());
    if (!playlistView) {
        std::cerr << "Error: PlaylistView is not available!\n";
        break;
    }
    playlistView->displayPlaylists(playlists);

    // Cho phép người dùng chọn playlist bằng ID
    int playlistId;
    std::cout << "\nEnter Playlist ID to add media '" << it->getName() << "': ";
    std::cin >> playlistId;

    // Kiểm tra ID hợp lệ
    if (playlistId <= 0 || playlistId > static_cast<int>(playlists.size())) {
        std::cerr << "Error: Invalid Playlist ID!\n";
        break;
    }

    // Lấy playlist được chọn
    Playlist& selectedPlaylist = playlists[playlistId - 1];

    // Thêm tệp phương tiện vào danh sách phát
    selectedPlaylist.addSong(*it);
    std::cout << "Media file '" << it->getName() << "' added to playlist '" << selectedPlaylist.getName() << "'.\n";

    // Lưu danh sách phát vào tệp
    try {
        playlistLibrary.saveToFile("playlists.txt");
        std::cout << "Updated playlist saved to file successfully.\n";
    } catch (const std::exception& e) {
        std::cerr << "Error saving playlist to file: " << e.what() << '\n';
    }

    // Hiển thị lại chi tiết danh sách phát
    playlistView->displayPlaylistDetails(selectedPlaylist);

    break;
}


    case 7:
        std::cout << "\nReturning Home...\n";
        ManagerController::getInstance().getManagerView()->setView("Default");
        //system("clear");
        break;
    default:
        std::cout << "Invalid choice! Please try again." << std::endl;
        break;
    }
}

