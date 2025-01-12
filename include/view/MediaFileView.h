#ifndef MEDIAFILEVIEW_H
#define MEDIAFILEVIEW_H

#include "common/BaseView.h"
#include <vector>
#include <string>

class MediaFileView : public BaseView {
public:
    void showMenu() override;
    int handleInput() override;
    void displayMediaFiles(const std::vector<std::string>& medialist, int page);
    void displayPagination(int currentPage, int totalPages);
    std::string promptDirectoryInput();
    void showOptionScan();
    int handleInputOptionScan();
};

#endif // MEDIAFILEVIEW_HPP
