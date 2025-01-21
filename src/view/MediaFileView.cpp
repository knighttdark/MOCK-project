#include "view/MediaFileView.h"
#include <common/TerminalUtils.h>
#include <common/Exception.h>
#include "controller/MediaFileController.h"
#include "common/Enum.h"

int MediaFileView::getSelectedMediaID() const {
    return selected_media_ID;
}

void MediaFileView::setSelectedMediaID(int id) {
    selected_media_ID = id;
}

int MediaFileView::showMenu() {
    MenuRenderer menu("Media File Menu",
                      {"1. Show Metadata",
                       "2. Play Song", "3. Add to Playlist","4. Return to Playing", "0. Return Home"},
                      {1, 2, 3, 4, 0});

    return menu.render();
}

void MediaFileView::displayMediaFiles(const vector<string>& medialist, int page, const string& notification_message) {
    if (medialist.empty()) {
        auto empty_renderer = Renderer([] {
            return vbox({
                text("No media files available.") | bold | center,
                separator(),
                text("Press ENTER to return.") | dim | center
            }) | center;
        });

        auto screen = ScreenInteractive::TerminalOutput();
        auto main_component = CatchEvent(empty_renderer, [&](Event event) {
            if (event == Event::Return) {
                screen.ExitLoopClosure()();
                return true;
            }
            return false;
        });

        screen.Loop(main_component);
        return;
    }

    int selected_index = 0;

    auto media_menu = Menu(&medialist, &selected_index);

    auto screen = ScreenInteractive::TerminalOutput();

    auto main_renderer = Renderer(media_menu, [&] {
        return vbox({
                   text("==== Media Files (Page " + std::to_string(page) +") ====") | bold | center,
                   separator(),
                   media_menu->Render() | border,
                   separator(),
                   text(notification_message) | color(Color::Green) | center,
                   separator(),
                   hbox({
                       text("Press ENTER to select | "),
                       text("Page Controls: <- Previous | -> Next ") | dim
                   }) | center
               }) |
               center;
    });

    bool next_page = false;
    bool previous_page = false;

    auto main_component = CatchEvent(main_renderer, [&](Event event) {
        if (event == Event::Return) {
            if (selected_index >= 0 && selected_index < (int)medialist.size()) {
                setSelectedMediaID(selected_index + 1);
                screen.ExitLoopClosure()();
            }
            return true;
        }

        if (event == Event::ArrowRight) {
            next_page = true;
            screen.ExitLoopClosure()();
            return true;
        }

        if (event == Event::ArrowLeft) {
            previous_page = true;
            screen.ExitLoopClosure()();
            return true;
        }

        if (event.is_mouse() && event.mouse().button == Mouse::Left && event.mouse().motion == Mouse::Pressed) {
            int clicked_index = event.mouse().y - 3;
            if (clicked_index >= 0 && clicked_index < (int)medialist.size()) {
                selected_index = clicked_index;
                setSelectedMediaID(selected_index + 1);
                screen.ExitLoopClosure()();
            }
            return true;
        }
        return media_menu->OnEvent(event);
    });

    screen.Loop(main_component);

    if (next_page) {
        MediaFileController* mediaFileController = dynamic_cast<MediaFileController*>(
            ManagerController::getInstance().getController("MediaFile"));

        /* Check if the MediaFileController is available */
        if (!mediaFileController) {
            cerr << "Error: MediaFileController is not available!" << endl;
        }
        mediaFileController ->handleAction(ACTION_NEXT_PAGE);
    } else if (previous_page) {
        MediaFileController* mediaFileController = dynamic_cast<MediaFileController*>(
            ManagerController::getInstance().getController("MediaFile"));

        /* Check if the MediaFileController is available */
        if (!mediaFileController) {
            cerr << "Error: MediaFileController is not available!" << endl;
        }
        mediaFileController ->handleAction(ACTION_PREVIOUS_PAGE);
    }
}

void MediaFileView::displayPagination(int currentPage, int totalPages) {
    cout << "\nPage " << currentPage << " of " << totalPages << '\n';
}

string MediaFileView::promptDirectoryInput() {
    string path = "";
    string error_message = "";

    auto input_box = Input(&path, "Enter directory path:");

    auto screen = ScreenInteractive::TerminalOutput();

    auto main_component = Renderer(input_box, [&] {
        return vbox({
            text("==== Enter Directory Path ====") | center,
            separator(),
            input_box->Render() | border,
            separator(),
            error_message.empty()
                ? text("Press Enter to confirm the input.") | dim | center
                : text(error_message) | color(Color::Red) | center,
        });
    });

    main_component = CatchEvent(main_component, [&](Event event) {
        if (event == Event::Return) {
            try {
                Exception::checkInputFilePath(path);
                screen.ExitLoopClosure()();
                return true;
            } catch (const invalid_argument& e) {
                error_message = e.what();
                return true;
            }
        }
        return false;
    });

    screen.Loop(main_component);

    return path;
}



int MediaFileView::showOptionScan() {
    vector<string> menu_entries = {
        "1. Scan Directory",
        "2. Scan USB",
        "0. Return Home"
    };

    vector<int> logic_mapping = {1, 2, 0};

    int selected = 0;
    string error_message;
    int final_selected = -1;

    auto menu = Menu(&menu_entries, &selected);

    auto screen = ScreenInteractive::TerminalOutput();

    auto main_component = Renderer(menu, [&] {
        return vbox({
            text("==== Scan Options ====") | center,
            separator(),
            menu->Render() | border,
            separator(),
            text("Use UP/DOWN keys, numbers (0-2), or click to navigate. Press ENTER to select.") | dim | center,
            separator(),
            text(error_message) | color(Color::Red) | center
        });
    });

    main_component = CatchEvent(main_component, [&](Event event) {
        if (event == Event::Return) {
            final_selected = logic_mapping[selected];
            screen.ExitLoopClosure()();
            return true;
        }

        if (event.is_character() && isdigit(event.character()[0])) {
            int number = event.character()[0] - '0';
            auto it = find(logic_mapping.begin(), logic_mapping.end(), number);
            if (it != logic_mapping.end()) {
                final_selected = number;
                screen.ExitLoopClosure()();
                return true;
            } else {
                error_message = "Invalid input: number not in menu!";
                return true;
            }
        }

        if (event.is_mouse() && event.mouse().button == Mouse::Left && event.mouse().motion == Mouse::Pressed) {
            int clicked_index = event.mouse().y - 3;
            if (clicked_index >= 0 && clicked_index < (int)menu_entries.size()) {
                final_selected = logic_mapping[clicked_index];
                screen.ExitLoopClosure()();
                return true;
            } else {
                error_message = "Invalid click: out of menu range!";
                return true;
            }
        }

        if (event == Event::ArrowUp || event == Event::ArrowDown) {
            menu->OnEvent(event);
            return true;
        }

        return false;
    });

    screen.Loop(main_component);

    return final_selected;
}