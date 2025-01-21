#ifndef MEDIA_FILE_CONTROLLER_H
#define MEDIA_FILE_CONTROLLER_H

#include "common/BaseController.h"
#include "model/MediaLibrary.h"
#include "view/MediaFileView.h"
#include "view/ManagerView.h"
#include "model/ManagerModel.h"
#include "controller/ManagerController.h"
#include "controller/MetadataController.h"
#include <iostream>

/* Manage media file actions */
class MediaFileController : public BaseController {
private:
    int currentPage = 0;
    const int pageSize = 25;

public:
    MediaFileController();
    void scanDirectory(const string& path);
    void scanUSBDevice();
    void handleActionScan(int option);
    void nextPage();
    void previousPage();
    void handleAction(int action) override;
    void scanAndDisplayMedia();
    string getPathById(const vector<MediaFile>& mediaFiles, int id);
};

#endif
