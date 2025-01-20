#include "controller/PlaylistController.h"
#include "controller/ManagerController.h"
#include "model/ManagerModel.h"
#include "common/Enum.h"
#include "common/Exception.h"
#include "view/PlaylistView.h"
#include <iostream>
#include <climits>
#include <controller/PlayingMediaController.h>
#include <bits/this_thread_sleep.h>


PlaylistController::PlaylistController() {}


void PlaylistController::handleAction(int action) {
    switch (action) {
        case ACTION_CREATE_PLAYLIST: {
            string name;
            while (true) {
            try {
                cout << "Enter a new playlist name: ";
                getline(cin, name); 
                Exception::checkStringNotEmpty(name, "Playlist name");
                break; 
            } catch (const invalid_argument& e) {
                cout << e.what() << endl; 
            }
            }
            createPlaylist(name);
            break;
        }
        case ACTION_DELETE_PLAYLIST: {
            
            cout << "Enter playlist name to delete: ";
            deletePlaylist();
            break;
        }
        case ACTION_VIEW_PLAYLIST_DETAILS: {
            
            string name;
            while (true) {
            try {
                cout << "Enter playlist name to view details: ";
                getline(cin, name); 
                Exception::checkStringNotEmpty(name, "Playlist name");
                break; 
            } catch (const invalid_argument& e) {
                cout << e.what() << endl; 
            }
            }
            viewPlaylistDetails(name);
            break;
        }
        case ACTION_LIST_ALL_PLAYLISTS:
            
            listAllPlaylists();
            break;
        case ACTION_PLAY_PLAYLISTS:
            {
            string name;
            while (true) {
            try {
                cout << "Enter playlist name to play: ";
                getline(cin, name); 
                Exception::checkStringNotEmpty(name, "Playlist name");
                break; 
            } catch (const invalid_argument& e) {
                cout << e.what() << endl; 
            }
            }
            playPlaylist(name);
            break;
        }

        case ACTION_EXIT_PLAYLIST_MENU:
            
            {
            cout << "Returning to previous menu.\n";
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
            cerr << "Invalid action.\n";
            break;
    }
}


void PlaylistController::createPlaylist(const string& name) {
    PlaylistLibrary& playlistLibrary = ManagerModel::getInstance().getPlaylistLibrary();

    if (playlistLibrary.getPlaylistByName(name) == nullptr) {
        playlistLibrary.addPlaylist(Playlist(name));
        cout << "Playlist '" << name << "' created successfully.\n";

        
        try {
            playlistLibrary.saveToFile("playlists.txt");
        } catch (const exception& e) {
            cerr << "Error saving playlist to file: " << e.what() << '\n';
        }
    } else {
        cerr << "Playlist with name '" << name << "' already exists.\n";
    }
}


void PlaylistController::deletePlaylist() {
    PlaylistLibrary& playlistLibrary = ManagerModel::getInstance().getPlaylistLibrary();
    auto& playlists = playlistLibrary.getPlaylists();

    
    if (playlists.empty()) {
        cout << "No playlists available to delete.\n";
        return;
    }

    
    PlaylistView* playlistView = dynamic_cast<PlaylistView*>(
        ManagerController::getInstance().getManagerView()->getView());
    if (!playlistView) {
        cerr << "Error: PlaylistView is not available.\n";
        return;
    }

    
    playlistView->displayPlaylists(playlists);

    
    int playlistId;
    do
    {
                cout << "\nEnter Playlist ID to delete: ";
                cin >> playlistId;
                if(playlistId < 1 || playlistId > static_cast<int>(playlists.size()))
                {
                    cout << "\nInvalid ID, please input ID from 1 to " << static_cast<int>(playlists.size()) << "!";
                    cin.ignore(INT_MAX, '\n');
                }
            } while (playlistId < 1 || playlistId > static_cast<int>(playlists.size()));

    
    const string playlistName = playlists[playlistId - 1].getName();
    playlistLibrary.removePlaylist(playlistName);
    cout << "Playlist '" << playlistName << "' deleted successfully.\n";

    
    try {
        playlistLibrary.saveToFile("playlists.txt");
        cout << "Updated playlists saved successfully to file.\n";
    } catch (const exception& e) {
        cerr << "Error saving updated playlists to file: " << e.what() << '\n';
    }
}


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

void PlaylistController::playPlaylist(const string& name) {
    PlaylistLibrary& playlistLibrary = ManagerModel::getInstance().getPlaylistLibrary();
    Playlist* playlist = playlistLibrary.getPlaylistByName(name);

    if (!playlist) {
        cerr << "Playlist '" << name << "' not found.\n";
        return;
    }

    const auto& songs = playlist->getSongs();

    if (songs.empty()) {
        cerr << "No songs in playlist '" << name << "'.\n";
        return;
    }
    ManagerController::getInstance().getManagerView()->setView("PlayingView");

    cout << "Playing playlist '" << name << "':\n";

    
    PlayingMediaController* playingMediaController = dynamic_cast<PlayingMediaController*>(
        ManagerController::getInstance().getController("PlayingView"));

    if (!playingMediaController) {
        cerr << "Error: PlayingMediaController is not available.\n";
        return;
    }

    
    playingMediaController->playPlaylist(const_cast<vector<MediaFile>&>(songs));
}
