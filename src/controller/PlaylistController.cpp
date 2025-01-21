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
            // Tạo biến để lưu tên playlist
            std::string name;
            std::string placeholder = "Enter new playlist name...";
            std::string result_message; // Thông báo kết quả
            Decorator message_style;    // Kiểu trang trí cho thông báo (màu xanh/đỏ)

            // Tạo Input Box
            auto input_box = Input(&name, placeholder);

            // Tạo Renderer cho Input Box
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

            // Tạo màn hình Interactive
            auto screen = ScreenInteractive::TerminalOutput();

            // Bắt sự kiện
            bool confirmed = false; // Flag xác nhận
            auto main_component = CatchEvent(input_renderer, [&](Event event) {
                if (event == Event::Return) {
                    confirmed = true; // Nhấn ENTER để xác nhận
                    screen.ExitLoopClosure()();
                    return true;
                }
                if (event == Event::Escape) {
                    confirmed = false; // Nhấn ESC để hủy
                    screen.ExitLoopClosure()();
                    return true;
                }
                return false;
            });

            // Chạy giao diện nhập tên
            screen.Loop(main_component);

            // Xử lý kết quả
            if (confirmed && !name.empty()) {
                createPlaylist(name); // Gọi hàm tạo playlist
                result_message = "Playlist '" + name + "' created successfully!";
                message_style = color(Color::Green); // Màu xanh cho thành công
            } else if (!confirmed) {
                result_message = "Playlist creation cancelled.";
                message_style = color(Color::Yellow); // Màu vàng cho hủy
            } else {
                result_message = "Error: Playlist name cannot be empty.";
                message_style = color(Color::Red); // Màu đỏ cho lỗi
            }

            // Hiển thị thông báo kết quả
            auto result_renderer = Renderer([&] {
                return vbox({
                    text(result_message) | bold | message_style | center,
                    separator(),
                    text("Press ENTER to continue...") | dim | center,
                }) | center;
            });

            // Bắt sự kiện nhấn ENTER để tiếp tục
            auto result_component = CatchEvent(result_renderer, [&](Event event) {
                if (event == Event::Return) {
                    screen.ExitLoopClosure()(); // Thoát màn hình
                    return true;
                }
                return false;
            });

            // Chạy giao diện thông báo kết quả
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

            // Lấy danh sách playlist từ model
            PlaylistLibrary& playlistLibrary = ManagerModel::getInstance().getPlaylistLibrary();
            const auto& playlists = playlistLibrary.getPlaylists();

            // Lấy tên playlist dựa trên ID
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

            // Lấy danh sách playlist từ model
            PlaylistLibrary& playlistLibrary = ManagerModel::getInstance().getPlaylistLibrary();
            const auto& playlists = playlistLibrary.getPlaylists();

            // Lấy tên playlist dựa trên ID
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
                text("Playlist View") | bold | center, // Main title
                separator(),
                text(notification_message) | (success ? color(Color::Green) : color(Color::Red)) | center, // Notification message
                separator(),
                text("Press ENTER to return to Playlist View.") | dim | center // User instruction
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
