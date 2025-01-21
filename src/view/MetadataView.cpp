#include "view/MetadataView.h"
#include "common/Exception.h"
#include <iostream>
#include <climits>

int MetadataView::showMenu() {
    MenuRenderer menu("Edit Metadata Menu",
                      {"1. Edit Title", "2. Edit Artist", "3. Edit Album",
                       "4. Edit Genre", "5. Edit Year", "0. Exit Editing"},
                      {1, 2, 3, 4, 5, 0});
    return menu.render();
}

void MetadataView::displayMetadata(const map<string, string>& metadata) {

    std::vector<Element> table_rows;

    table_rows.push_back(
        hbox({
            text("Field") | bold | size(WIDTH, EQUAL, 20),
            text("Value") | bold | size(WIDTH, EQUAL, 40)
        }) | border
    );

    for (const auto& [key, value] : metadata) {
        std::string remaining_value = value;
        bool first_row = true;

        while (!remaining_value.empty()) {
            std::string current_value = remaining_value.substr(0, 40);
            remaining_value = remaining_value.length() > 40 ? remaining_value.substr(40) : "";

            table_rows.push_back(
                hbox({
                    text(first_row ? key : "") | size(WIDTH, EQUAL, 20),
                    text(current_value) | size(WIDTH, EQUAL, 40)
                })
            );

            first_row = false;
        }
    }

    auto table = vbox(std::move(table_rows)) | border;

    auto renderer = Renderer([&] {
        return vbox({
            text("Metadata Viewer") | bold | center,
            separator(),
            table | center,
            separator(),
            text("Press ENTER to return.") | dim | center
        }) | center;
    });

    auto screen = ScreenInteractive::TerminalOutput();
    auto main_component = CatchEvent(renderer, [&](Event event) {
        if (event == Event::Return) {
            screen.ExitLoopClosure()();
            return true;
        }
        return false;
    });

    screen.Loop(main_component);
}
