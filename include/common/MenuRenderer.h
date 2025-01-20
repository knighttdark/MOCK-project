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
    string title;                        // Tiêu đề menu
    vector<string> menu_entries;   // Danh sách các mục menu
    vector<int> logic_mapping;          // Ánh xạ logic cho các mục menu

public:
    // Constructor
    MenuRenderer(const string& title, const vector<string>& menu_entries, const vector<int>& logic_mapping);

    // Phương thức render menu và xử lý sự kiện
    int render();
};

#endif // MENU_RENDERER_H
