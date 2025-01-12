#include "controller/PlaylistController.h"
#include "model/ManagerModel.h"
#include "controller/ManagerController.h"
#include "view/PlaylistView.h"
#include <iostream>

// Constructor
PlaylistController::PlaylistController() {}

// Override handleAction
void PlaylistController::handleAction(int action) {
    switch (action) {
    case 1: // Create a new playlist
        std::cout << "Enter new playlist name: ";
        {
            std::string name;
            std::cin >> name;
            createPlaylist(name);
        }
        break;
    case 2: // Delete a playlist
        std::cout << "Enter playlist name to delete: ";
        {
            
            deletePlaylist();
        break;
        }
    case 3: // View playlist details
        std::cout << "Enter playlist name to view details: ";
        {
            std::string name;
            std::cin >> name;
            viewPlaylistDetails(name);
        }
        break;
    case 4: // List all playlists
        listAllPlaylists();
        break;
    case 0: // Exit
        std::cout << "Returning to previous menu.\n";
        ManagerController::getInstance().getManagerView()->setView("Default");
        break;
    default:
        std::cerr << "Invalid action.\n";
        break;
    }
}

// Create a new playlist
void PlaylistController::createPlaylist(const std::string& name) {
    PlaylistLibrary& playlistLibrary = ManagerModel::getInstance().getPlaylistLibrary();

    if (playlistLibrary.getPlaylistByName(name) == nullptr) {
        playlistLibrary.addPlaylist(Playlist(name));
        std::cout << "Playlist '" << name << "' created successfully.\n";

        // Lưu vào tệp
        try {
            playlistLibrary.saveToFile("playlists.txt");
        } catch (const std::exception& e) {
            std::cerr << "Error saving playlist to file: " << e.what() << '\n';
        }
    } else {
        std::cerr << "Playlist with name '" << name << "' already exists.\n";
    }
}


// Delete a playlist
void PlaylistController::deletePlaylist() {
    PlaylistLibrary& playlistLibrary = ManagerModel::getInstance().getPlaylistLibrary();
    auto& playlists = playlistLibrary.getPlaylists();

    // Kiểm tra nếu danh sách playlist trống
    if (playlists.empty()) {
        std::cout << "No playlists available to delete.\n";
        return;
    }

    // Lấy view hiện tại
    PlaylistView* playlistView = dynamic_cast<PlaylistView*>(
        ManagerController::getInstance().getManagerView()->getView());
    if (!playlistView) {
        std::cerr << "Error: PlaylistView is not available.\n";
        return;
    }

    // Hiển thị danh sách playlist
    playlistView->displayPlaylists(playlists);

    // Yêu cầu người dùng nhập ID playlist cần xóa
    int playlistId;
    std::cout << "\nEnter Playlist ID to delete: ";
    std::cin >> playlistId;

    // Kiểm tra ID hợp lệ
    if (playlistId <= 0 || playlistId > static_cast<int>(playlists.size())) {
        std::cerr << "Error: Invalid Playlist ID!\n";
        return;
    }

    // Xóa playlist theo ID
    const std::string playlistName = playlists[playlistId - 1].getName();
    playlistLibrary.removePlaylist(playlistName);
    std::cout << "Playlist '" << playlistName << "' deleted successfully.\n";

    // Lưu thông tin cập nhật vào tệp
    try {
        playlistLibrary.saveToFile("playlists.txt");
        std::cout << "Updated playlists saved successfully to file.\n";
    } catch (const std::exception& e) {
        std::cerr << "Error saving updated playlists to file: " << e.what() << '\n';
    }
}

// View details of a specific playlist
void PlaylistController::viewPlaylistDetails(const std::string& name) {
    PlaylistLibrary& playlistLibrary = ManagerModel::getInstance().getPlaylistLibrary();
    Playlist* playlist = playlistLibrary.getPlaylistByName(name);
    if (!playlist) {
        std::cerr << "Playlist '" << name << "' not found.\n";
        return;
    }

    PlaylistView* playlistView = dynamic_cast<PlaylistView*>(
        ManagerController::getInstance().getManagerView()->getView());
    if (!playlistView) {
        std::cerr << "Error: PlaylistView is not available.\n";
        return;
    }

    playlistView->displayPlaylistDetails(*playlist);
}

// List all playlists
void PlaylistController::listAllPlaylists() {
    PlaylistLibrary& playlistLibrary = ManagerModel::getInstance().getPlaylistLibrary();
    PlaylistView* playlistView = dynamic_cast<PlaylistView*>(
        ManagerController::getInstance().getManagerView()->getView());

    if (!playlistView) {
        std::cerr << "Error: PlaylistView is not available.\n";
        return;
    }

     auto& playlists = playlistLibrary.getPlaylists();
    if (playlists.empty()) {
        std::cout << "No playlists available.\n";
    } else {
        playlistView->displayPlaylists(playlists);
    }
}
