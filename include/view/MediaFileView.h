#ifndef MEDIAFILEVIEW_H
#define MEDIAFILEVIEW_H

#include "common/BaseView.h"
#include <vector>
#include <string>

/* View for displaying media files */
class MediaFileView : public BaseView {
public:
    void showMenu() override; /* Show menu */
    int handleInput() override; /* Handle input */
    void displayMediaFiles(const vector<string>& medialist, int page); /* Display media files */
    void displayPagination(int currentPage, int totalPages); /* Display pagination */
    string promptDirectoryInput(); /* Prompt directory input */
    void showOptionScan(); /* Show scan options */
    int handleInputOptionScan(); /* Handle scan option input */
};

#endif /* MEDIAFILEVIEW_H */
