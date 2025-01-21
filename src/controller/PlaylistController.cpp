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
PlaylistController::PlaylistController() {}

void PlaylistController::handleAction(int action) {
    switch (action) {
        case ACTION_CREATE_PLAYLIST: {
            std::string name;
            std::string placeholder = "Enter new playlist name...";
            std::string result_message;
            Decorator message_style;

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

            auto result_renderer = Renderer([&] {
                return vbox({
                    text(result_message) | bold | message_style | center,
                    separator(),
                    text("Press ENTER to continue...") | dim | center,
                }) | center;
            });

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
            deletePlaylist();
            clearTerminal();
            listAllPlaylists();
            break;
        }
        case ACTION_VIEW_PLAYLIST_DETAILS: {
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
            clearTerminal();
            listAllPlaylists();
            break;
        case ACTION_PLAY_PLAYLISTS:
            {
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

        case ACTION_EXIT_PLAYLIST_MENU:
            {
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

    /* Get the selected playlist ID from the view */
    int selected_playlist_ID = playlistView->getSelectedPlaylistID();

    /* Validate the selected playlist ID */
    if (selected_playlist_ID <= 0 || selected_playlist_ID > static_cast<int>(playlists.size())) {
        cerr << "Error: Invalid Playlist ID!\n";
        return;
    }

    /* Remove the playlist by ID */
    const std::string playlistName = playlists[selected_playlist_ID - 1].getName();
        playlistLibrary.removePlaylist(playlistName);

        std::string notification_message = "Playlist '" + playlistName + "' deleted successfully.";
        bool success = true;

        try {
            playlistLibrary.saveToFile("playlists.txt");
            notification_message += "\nUpdated playlists saved successfully to file.";
        } catch (const std::exception& e) {
            notification_message = "Error saving updated playlists to file: " + std::string(e.what());
            success = false;
        }

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

        auto main_component = CatchEvent(notification_renderer, [&](Event event) {
            if (event == Event::Return) {
                screen.ExitLoopClosure()();
                return true;
            }
            return false;
        });

        screen.Loop(main_component);
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
