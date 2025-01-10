#ifndef DEFAULT_SCREEN_CONTROLLER_H
#define DEFAULT_SCREEN_CONTROLLER_H

#include "common/BaseController.h"
#include "view/ManagerView.h"
#include "controller/ManagerController.h"
#include <iostream>

class DefaultScreenController : public BaseController {
private:
   

public:
    DefaultScreenController();
    void handleAction(int action) override;
};

#endif 