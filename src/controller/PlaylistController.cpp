#include "controller/PlaylistController.h"
#include "model/MediaFile.h"
#include <iostream>
#include <limits>

PlaylistController::PlaylistController()
    : playlistView(new PlaylistView()),
      playlistLibrary(ManagerModel::getInstance().getPlaylistLibrary()) {}

PlaylistController::~PlaylistController() { delete playlistView; }

void PlaylistController::handleAction(int action) {
    switch (action) {
    case 1: { // Create Playlist
        std::cout << "Enter playlist name: ";
        std::string name;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Xóa buffer
        std::getline(std::cin, name);
        if (name.empty()) {
            std::cout << "Playlist name cannot be empty.\n";
        } else {
            playlistLibrary.addPlaylist(name);
            std::cout << "Playlist created: " << name << "\n";
        }
        break;
    }
    case 2: { // View Playlist
        std::cout << "Enter playlist name: ";
        std::string name;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::getline(std::cin, name);
        Playlist* playlist = playlistLibrary.getPlaylist(name);
        if (playlist) {
            playlistView->displayPlaylistDetails(*playlist);
        } else {
            std::cout << "Playlist not found.\n";
        }
        break;
    }
    case 3: { // Add Media to Playlist
        std::cout << "Enter playlist name: ";
        std::string playlistName;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::getline(std::cin, playlistName);
        Playlist* playlist = playlistLibrary.getPlaylist(playlistName);
        if (playlist) {
            std::cout << "Enter media details:\n";
            std::cout << "Media ID: ";
            int id;
            std::cin >> id;

            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Clear buffer
            std::cout << "Media Name: ";
            std::string name;
            std::getline(std::cin, name);

            std::cout << "Media Artist: ";
            std::string artist;
            std::getline(std::cin, artist);

            std::cout << "Media Genre: ";
            std::string genre;
            std::getline(std::cin, genre);

            if (!name.empty() && !artist.empty() && !genre.empty()) {
                MediaFile mediaFile(id, name, artist, genre.c_str());
                playlist->addMediaFile(mediaFile);
                std::cout << "Media added to playlist: " << playlistName << "\n";
            } else {
                std::cout << "All fields are required. Media not added.\n";
            }
        } else {
            std::cout << "Playlist not found.\n";
        }
        break;
    }
    default:
        std::cout << "Invalid option.\n";
    }
}
