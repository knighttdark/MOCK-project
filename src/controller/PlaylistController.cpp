#include "controller/PlaylistController.h"
#include "controller/ManagerController.h"
#include "model/ManagerModel.h"
#include "common/Enum.h"
#include "view/PlaylistView.h"
#include <iostream>
#include <controller/PlayingMediaController.h>
#include <bits/this_thread_sleep.h>

/* Constructor for PlaylistController */
PlaylistController::PlaylistController() {}

/* Handle actions based on user input */
void PlaylistController::handleAction(int action) {
    switch (action) {
        case ACTION_CREATE_PLAYLIST: {
            /* Create a new playlist */
            cout << "Enter new playlist name: ";
            string name;
            cin >> name;
            createPlaylist(name);
            break;
        }
        case ACTION_DELETE_PLAYLIST: {
            /* Delete a playlist */
            cout << "Enter playlist name to delete: ";
            deletePlaylist();
            break;
        }
        case ACTION_VIEW_PLAYLIST_DETAILS: {
            /* View playlist details */
            cout << "Enter playlist name to view details: ";
            string name;
            cin >> name;
            viewPlaylistDetails(name);
            break;
        }
        case ACTION_LIST_ALL_PLAYLISTS:
            /* List all playlists */
            listAllPlaylists();
            break;
        case ACTION_PLAY_PLAYLISTS:
            {
            cout << "Enter playlist name to play: ";
            string name;
            cin >> name;
            playPlaylist(name);
            break;
        }

        case ACTION_EXIT_PLAYLIST_MENU:
            /* Exit to the previous menu */
            {cout << "Returning to previous menu.\n";
            PlayingMediaController* playingController = dynamic_cast<PlayingMediaController*>(
            ManagerController::getInstance().getController("PlayingView"));

            if (!playingController) {
                std::cerr << "Error: PlayingMediaController not available!\n";
                break;}
            playingController->stop();
            ManagerController::getInstance().getManagerView()->setView("Default");
            break;}
        default:
            cerr << "Invalid action.\n";
            break;
    }
}

/* Create a new playlist */
void PlaylistController::createPlaylist(const string& name) {
    PlaylistLibrary& playlistLibrary = ManagerModel::getInstance().getPlaylistLibrary();

    if (playlistLibrary.getPlaylistByName(name) == nullptr) {
        playlistLibrary.addPlaylist(Playlist(name));
        cout << "Playlist '" << name << "' created successfully.\n";

        /* Save to file */
        try {
            playlistLibrary.saveToFile("playlists.txt");
        } catch (const exception& e) {
            cerr << "Error saving playlist to file: " << e.what() << '\n';
        }
    } else {
        cerr << "Playlist with name '" << name << "' already exists.\n";
    }
}

/* Delete a playlist */
void PlaylistController::deletePlaylist() {
    PlaylistLibrary& playlistLibrary = ManagerModel::getInstance().getPlaylistLibrary();
    auto& playlists = playlistLibrary.getPlaylists();

    /* Check if the playlist list is empty */
    if (playlists.empty()) {
        cout << "No playlists available to delete.\n";
        return;
    }

    /* Get the current view */
    PlaylistView* playlistView = dynamic_cast<PlaylistView*>(
        ManagerController::getInstance().getManagerView()->getView());
    if (!playlistView) {
        cerr << "Error: PlaylistView is not available.\n";
        return;
    }

    /* Display the list of playlists */
    playlistView->displayPlaylists(playlists);

    /* Ask the user to enter the playlist ID to delete */
    int playlistId;
    cout << "\nEnter Playlist ID to delete: ";
    cin >> playlistId;

    /* Validate the playlist ID */
    if (playlistId <= 0 || playlistId > static_cast<int>(playlists.size())) {
        cerr << "Error: Invalid Playlist ID!\n";
        return;
    }

    /* Remove the playlist by ID */
    const string playlistName = playlists[playlistId - 1].getName();
    playlistLibrary.removePlaylist(playlistName);
    cout << "Playlist '" << playlistName << "' deleted successfully.\n";

    /* Save the updated playlists to file */
    try {
        playlistLibrary.saveToFile("playlists.txt");
        cout << "Updated playlists saved successfully to file.\n";
    } catch (const exception& e) {
        cerr << "Error saving updated playlists to file: " << e.what() << '\n';
    }
}

/* View details of a specific playlist */
void PlaylistController::viewPlaylistDetails(const string& name) {
    PlaylistLibrary& playlistLibrary = ManagerModel::getInstance().getPlaylistLibrary();
    Playlist* playlist = playlistLibrary.getPlaylistByName(name);
    if (!playlist) {
        cerr << "Playlist '" << name << "' not found.\n";
        return;
    }

    PlaylistView* playlistView = dynamic_cast<PlaylistView*>(
        ManagerController::getInstance().getManagerView()->getView());
    if (!playlistView) {
        cerr << "Error: PlaylistView is not available.\n";
        return;
    }

    playlistView->displayPlaylistDetails(*playlist);
}

/* List all playlists */
void PlaylistController::listAllPlaylists() {
    PlaylistLibrary& playlistLibrary = ManagerModel::getInstance().getPlaylistLibrary();
    PlaylistView* playlistView = dynamic_cast<PlaylistView*>(
        ManagerController::getInstance().getManagerView()->getView());

    if (!playlistView) {
        cerr << "Error: PlaylistView is not available.\n";
        return;
    }

    auto& playlists = playlistLibrary.getPlaylists();
    if (playlists.empty()) {
        cout << "No playlists available.\n";
    } else {
        playlistView->displayPlaylists(playlists);
    }
}

void PlaylistController::playPlaylist(const std::string& name) {
    PlaylistLibrary& playlistLibrary = ManagerModel::getInstance().getPlaylistLibrary();
    Playlist* playlist = playlistLibrary.getPlaylistByName(name);

    if (!playlist) {
        std::cerr << "Playlist '" << name << "' not found.\n";
        return;
    }

    const auto& songs = playlist->getSongs();

    if (songs.empty()) {
        std::cerr << "No songs in playlist '" << name << "'.\n";
        return;
    }
    ManagerController::getInstance().getManagerView()->setView("PlayingView");

    std::cout << "Playing playlist '" << name << "':\n";

    // Lấy PlayingMediaController từ ManagerController
    PlayingMediaController* playingMediaController = dynamic_cast<PlayingMediaController*>(
        ManagerController::getInstance().getController("PlayingView"));

    if (!playingMediaController) {
        std::cerr << "Error: PlayingMediaController is not available.\n";
        return;
    }

    // Chuyển danh sách bài hát sang PlayingMediaController
    playingMediaController->playPlaylist(const_cast<std::vector<MediaFile>&>(songs));
}




    // Lấy PlayingMediaController từ ManagerController
    // PlayingMediaController* playingMediaController = dynamic_cast<PlayingMediaController*>(
    //     ManagerController::getInstance().getController("PlayingView"));