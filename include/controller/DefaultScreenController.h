#ifndef DEFAULT_SCREEN_CONTROLLER_H
#define DEFAULT_SCREEN_CONTROLLER_H

#include "common/BaseController.h"
#include "view/ManagerView.h"
#include <iostream>

class DefaultScreenController : public BaseController {
private:
    ManagerView* manager;

public:
    DefaultScreenController(ManagerView* managerView);
    void handleAction(int action) override;
};

#endif 