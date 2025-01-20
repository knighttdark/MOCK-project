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
private:
    int selected_media_ID = -1; // ID của file media được chọn (default: -1)

public:
    // Getter và Setter cho selected_media_ID
    int getSelectedMediaID() const;
    void setSelectedMediaID(int id);

    // void showMenu() override; /* Show menu */
    // int handleInput() override; /* Handle input */
    int showMenu() override; /* Show menu */
    void displayMediaFiles(const vector<string>& medialist, int page, const string& notification_message); /* Display media files */
    void displayPagination(int currentPage, int totalPages); /* Display pagination */
    string promptDirectoryInput(); /* Prompt directory input */
    // void showOptionScan(); /* Show scan options */
    // int handleInputOptionScan(); /* Handle scan option input */
    int showOptionScan(); /* Hiển thị menu */
};

#endif /* MEDIAFILEVIEW_H */
