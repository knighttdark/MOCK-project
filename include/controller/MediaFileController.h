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
    int currentPage = 0; /* Current page */
    const int pageSize = 25; /* Files per page */

public:
    MediaFileController(); /* Constructor */
    void scanDirectory(const string& path); /* Scan directory */
    void scanUSBDevice(); /* Scan USB */
    void handleActionScan(int option); /* Handle scan action */
    void nextPage(); /* Next page */
    void previousPage(); /* Previous page */
    void handleAction(int action) override; /* Handle user actions */
    void scanAndDisplayMedia(); /* Scan and display media */
    string getPathById(const vector<MediaFile>& mediaFiles, int id); /* Get path by ID */
};

#endif /* MEDIA_FILE_CONTROLLER_H */
