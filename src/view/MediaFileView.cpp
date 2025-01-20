#include "view/MediaFileView.h"

#include <common/Exception.h>

int MediaFileView::showMenu() {
    MenuRenderer menu("Media File Menu",
                      {"1. Show Metadata", "2. Next Page", "3. Previous Page",
                       "4. Play by ID", "5. Add to Playlist","6. Return to Playing", "0. Return Home"},
                      {1, 2, 3, 4, 5, 6, 0});
    return menu.render();
}


void MediaFileView::displayMediaFiles(const vector<string>& medialist, int page) {
    cout << "\n==== Media Files (Page " << page << ") ====\n";
    for (const auto& file : medialist) {
        cout << file << '\n';
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