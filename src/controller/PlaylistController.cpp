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
#include "common/TerminalUtils.h"

/* Default constructor for PlaylistController */
PlaylistController::PlaylistController() {}

/* Function to handle user actions in the playlist view */
void PlaylistController::handleAction(int action) {
    switch (action) {
        case ACTION_CREATE_PLAYLIST: {
            /* Create a new playlist */
            string name;
            string placeholder = "Enter new playlist name...";
            string result_message;
            Decorator message_style;

            /* Display an input box for the user to enter the playlist name */
            auto input_box = Input(&name, placeholder);
            auto input_renderer = Renderer(input_box, [&] {
                return vbox({
                    text("Create New Playlist") | bold | center,
                    separator(),
                    text("Enter playlist name:") | center,
                    input_box->Render() | border,
                    separator(),
                    text("Press ENTER to confirm, ESC to cancel.") | dim | center,
                }) | center;
            });

            auto screen = ScreenInteractive::TerminalOutput();
            bool confirmed = false;

            /* Capture user input for playlist name */
            auto main_component = CatchEvent(input_renderer, [&](Event event) {
                if (event == Event::Return) {
                    confirmed = true;
                    screen.ExitLoopClosure()();
                    return true;
                }
                if (event == Event::Escape) {
                    confirmed = false;
                    screen.ExitLoopClosure()();
                    return true;
                }
                return false;
            });

            screen.Loop(main_component);

            /* Handle confirmed or cancelled actions */
            if (confirmed && !name.empty()) {
                createPlaylist(name);
                result_message = "Playlist '" + name + "' created successfully!";
                message_style = color(Color::Green);
            } else if (!confirmed) {
                result_message = "Playlist creation cancelled.";
                message_style = color(Color::Yellow);
            } else {
                result_message = "Error: Playlist name cannot be empty.";
                message_style = color(Color::Red);
            }

            /* Display the result message */
            auto result_renderer = Renderer([&] {
                return vbox({
                    text(result_message) | bold | message_style | center,
                    separator(),
                    text("Press ENTER to continue...") | dim | center,
                }) | center;
            });

            /* Capture user input to continue */
            auto result_component = CatchEvent(result_renderer, [&](Event event) {
                if (event == Event::Return) {
                    screen.ExitLoopClosure()();
                    return true;
                }
                return false;
            });

            screen.Loop(result_component);
            clearTerminal();
            listAllPlaylists();
            break;
        }
        case ACTION_DELETE_PLAYLIST: {
            /* Delete a playlist */
            deletePlaylist();
            clearTerminal();
            listAllPlaylists();
            break;
        }
        case ACTION_VIEW_PLAYLIST_DETAILS: {
            /* Displays details of a selected playlist */
            PlaylistView* playlistView = dynamic_cast<PlaylistView*>(ManagerController::getInstance().getManagerView()->getView());
            if (!playlistView) {
                cerr << "Error: PlaylistView is not available.\n";
                break;
            }

            int selected_playlist_ID = playlistView->getSelectedPlaylistID();

            PlaylistLibrary& playlistLibrary = ManagerModel::getInstance().getPlaylistLibrary();
            const auto& playlists = playlistLibrary.getPlaylists();

            const string& selected_playlist_name = playlists[selected_playlist_ID - 1].getName();

            viewPlaylistDetails(selected_playlist_name);
            clearTerminal();
            listAllPlaylists();
            break;
        }
        case ACTION_LIST_ALL_PLAYLISTS:
            /* List all playlists */
            clearTerminal();
            listAllPlaylists();
            break;
        case ACTION_PLAY_PLAYLISTS: {
            /* Play the selected playlist */
            PlaylistView* playlistView = dynamic_cast<PlaylistView*>(ManagerController::getInstance().getManagerView()->getView());
            if (!playlistView) {
                cerr << "Error: PlaylistView is not available.\n";
                break;
            }

            int selected_playlist_ID = playlistView->getSelectedPlaylistID();
            if (selected_playlist_ID <= 0) {
                cerr << "Error: No playlist selected.\n";
                break;
            }

            PlaylistLibrary& playlistLibrary = ManagerModel::getInstance().getPlaylistLibrary();
            const auto& playlists = playlistLibrary.getPlaylists();

            const string& selected_playlist_name = playlists[selected_playlist_ID - 1].getName();
            playPlaylist(selected_playlist_name);
            break;
        }

        case ACTION_EXIT_PLAYLIST_MENU: {
            /* Stop the current media and return to the home screen */
            PlayingMediaController* playingController = dynamic_cast<PlayingMediaController*>(
                ManagerController::getInstance().getController("PlayingView"));

            if (!playingController) {
                cerr << "Error: PlayingMediaController not available!\n";
                break;}
            playingController->stop();
            clearTerminal();
            ManagerController::getInstance().getManagerView()->setView("Default");
            break;
        }
        default:
            cerr << "Invalid action.\n";
            break;
    }
}

/* Create a new playlist with the given name */
void PlaylistController::createPlaylist(const string& name) {
    PlaylistLibrary& playlistLibrary = ManagerModel::getInstance().getPlaylistLibrary();

    /* Check if a playlist with the same name already exists */
    if (playlistLibrary.getPlaylistByName(name) == nullptr) {
        playlistLibrary.addPlaylist(Playlist(name));
        cout << "Playlist '" << name << "' created successfully.\n";

        /* Save the updated playlists to file */
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

    /* Get the selected playlist ID from the view */
    int selected_playlist_ID = playlistView->getSelectedPlaylistID();
    if (selected_playlist_ID <= 0 || selected_playlist_ID > static_cast<int>(playlists.size())) {
        cerr << "Error: Invalid Playlist ID!\n";
        return;
    }

    /* Remove the playlist by ID */
    const string playlistName = playlists[selected_playlist_ID - 1].getName();
    playlistLibrary.removePlaylist(playlistName);

    string notification_message = "Playlist '" + playlistName + "' deleted successfully.";
    bool success = true;

    try {
        playlistLibrary.saveToFile("playlists.txt");
        notification_message += "\nUpdated playlists saved successfully to file.";
    } catch (const exception& e) {
        notification_message = "Error saving updated playlists to file: " + string(e.what());
        success = false;
    }

    /* Display a notification to the user */
    auto screen = ScreenInteractive::TerminalOutput();
    auto notification_renderer = Renderer([&] {
        return vbox({
            text("Playlist View") | bold | center,
            separator(),
            text(notification_message) | (success ? color(Color::Green) : color(Color::Red)) | center,
            separator(),
            text("Press ENTER to return to Playlist View.") | dim | center
        });
    });

    /* Capture user input to return to the playlist view */
    auto main_component = CatchEvent(notification_renderer, [&](Event event) {
        if (event == Event::Return) {
            screen.ExitLoopClosure()();
            return true;
        }
        return false;
    });

    screen.Loop(main_component);
}

/* View details of a playlist by name */
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

/* List all available playlists */
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

/* Play the selected playlist */
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

    /* Switch to the PlayingView */
    ManagerController::getInstance().getManagerView()->setView("PlayingView");
    cout << "Playing playlist '" << name << "':\n";

    PlayingMediaController* playingMediaController = dynamic_cast<PlayingMediaController*>(
        ManagerController::getInstance().getController("PlayingView"));

    if (!playingMediaController) {
        cerr << "Error: PlayingMediaController is not available.\n";
        return;
    }

    /* Play the selected playlist */
    playingMediaController->playPlaylist(const_cast<vector<MediaFile>&>(songs));
}
