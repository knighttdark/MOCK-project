#ifndef MENU_RENDERER_H
#define MENU_RENDERER_H

#include <string>
#include <vector>
#include <ftxui/component/component.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/dom/elements.hpp>

using namespace std;

class MenuRenderer {
private:
    string title;                        
    vector<string> menu_entries;   
    vector<int> logic_mapping;          

public:
    
    MenuRenderer(const string& title, const vector<string>& menu_entries, const vector<int>& logic_mapping);

    
    int render();
};

#endif 
