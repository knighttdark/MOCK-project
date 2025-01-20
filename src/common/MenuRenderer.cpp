#include "common/MenuRenderer.h"
#include <algorithm>
#include <iostream>
#include <cctype>

using namespace ftxui;

MenuRenderer::MenuRenderer(const string& title, const vector<string>& menu_entries, const vector<int>& logic_mapping)
    : title(title), menu_entries(menu_entries), logic_mapping(logic_mapping) {}

int MenuRenderer::render() {
    int selected = 0;               
    string error_message;      
    int final_selected = -1;        

    
    auto menu = Menu(&menu_entries, &selected);
    auto screen = ScreenInteractive::TerminalOutput();

    
    auto main_component = Renderer(menu, [&] {
        return vbox({
            text(title) | center,                        
            separator(),                                 
            menu->Render() | border,                    
            separator(),
            text("Use UP/DOWN keys, numbers, or click to navigate. Press ENTER to select.") | dim | center,
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

        
        if (event == Event::ArrowUp || event == Event::ArrowDown) {
            menu->OnEvent(event);                   
            return true;
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

        return false; 
    });

    
    screen.Loop(main_component);

    
    return final_selected;
}
