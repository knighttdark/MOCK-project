#ifndef MEDIAFILEVIEW_H
#define MEDIAFILEVIEW_H
#include "common/MenuRenderer.h"
#include "common/BaseView.h"
#include <vector>
#include <string>
#include <ftxui/component/component.hpp>  // For interactive components.
#include <ftxui/component/screen_interactive.hpp>  // For ScreenInteractive.
#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;
using namespace ftxui;
/* View for displaying media files */
class MediaFileView : public BaseView {
public:
    // void showMenu() override; /* Show menu */
    // int handleInput() override; /* Handle input */
    int showMenu() override; /* Show menu */
    void displayMediaFiles(const vector<string>& medialist, int page); /* Display media files */
    void displayPagination(int currentPage, int totalPages); /* Display pagination */
    string promptDirectoryInput(); /* Prompt directory input */
    // void showOptionScan(); /* Show scan options */
    // int handleInputOptionScan(); /* Handle scan option input */
    int showOptionScan(); /* Hiển thị menu */
};

#endif /* MEDIAFILEVIEW_H */
