#ifndef DEFAULT_SCREEN_CONTROLLER_H
#define DEFAULT_SCREEN_CONTROLLER_H

#include "common/BaseController.h"
#include "view/ManagerView.h"
#include "controller/ManagerController.h"
#include <iostream>

/* DefaultScreenController class to handle actions in the default screen view */
class DefaultScreenController : public BaseController {
private:
     ManagerController* managerController;
     bool isTestMode;
public:
    DefaultScreenController();
    explicit DefaultScreenController(ManagerController* controller,bool testMode = false ):isTestMode(testMode){
        if (controller) {
            managerController = controller;
        } else {
            managerController = &ManagerController::getInstance();
        }
    }
    void handleAction(int action) override;
};

#endif /* DEFAULT_SCREEN_CONTROLLER_H */
