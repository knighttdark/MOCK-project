#ifndef MENU_RENDERER_H
#define MENU_RENDERER_H

#include <string>
#include <vector>
#include <ftxui/component/component.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/dom/elements.hpp>

class MenuRenderer {
private:
    std::string title;                        // Tiêu đề menu
    std::vector<std::string> menu_entries;   // Danh sách các mục menu
    std::vector<int> logic_mapping;          // Ánh xạ logic cho các mục menu

public:
    // Constructor
    MenuRenderer(const std::string& title, const std::vector<std::string>& menu_entries, const std::vector<int>& logic_mapping);

    // Phương thức render menu và xử lý sự kiện
    int render();
};

#endif // MENU_RENDERER_H
