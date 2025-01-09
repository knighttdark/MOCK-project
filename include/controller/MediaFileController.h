#ifndef MEDIA_FILE_CONTROLLER_H
#define MEDIA_FILE_CONTROLLER_H

#include "common/BaseController.h"
#include "view/ManagerView.h"
#include <iostream>

class MediaFileController : public BaseController {
private:
    ManagerView* manager;

public:
    MediaFileController(ManagerView* managerView);
    void handleAction(int action) override;
};

#endif // MEDIA_FILE_CONTROLLER_HPP
