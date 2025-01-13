#ifndef DEFAULT_SCREEN_CONTROLLER_H
#define DEFAULT_SCREEN_CONTROLLER_H

#include "common/BaseController.h"
#include "view/ManagerView.h"
#include "controller/ManagerController.h"
#include <iostream>

/* DefaultScreenController class to handle actions in the default screen view */
class DefaultScreenController : public BaseController {
private:

public:
    /* Constructor for DefaultScreenController */
    DefaultScreenController();

    /* Handle an action based on the provided action code */
    void handleAction(int action) override;
};

#endif /* DEFAULT_SCREEN_CONTROLLER_H */
