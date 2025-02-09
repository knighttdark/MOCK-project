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

    ManagerController* managerController;
     bool isTestMode;
public:
    MediaFileController();

    explicit MediaFileController(ManagerController* controller)
        : managerController(controller ? controller : &ManagerController::getInstance()) {}

    void scanDirectory(const string& path);
    void scanUSBDevice();
    virtual void handleActionScan(int option);
    //virtual void handleActionScan(int option, std::function<std::vector<std::string>(const std::string&)> listUSBDevices);

    void nextPage();
    void previousPage();
     void handleAction(int action) ;
    virtual void scanAndDisplayMedia();
    string getPathById(const vector<MediaFile>& mediaFiles, int id);
};

#endif
