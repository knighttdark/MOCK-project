#include "controller/ManagerController.h"
#include <iostream>
#include "view/DefaultScreenView.h"
#include "controller/DefaultScreenController.h"
#include "view/MediaFileView.h"
#include "controller/MediaFileController.h"

// Private constructor
ManagerController::ManagerController() {}

// Static method to get the instance
ManagerController& ManagerController::getInstance() {
    static ManagerController instance;
    return instance;
}

void ManagerController::registerController(const std::string& key, BaseController* controller) {
    controllers[key] = controller;
}

void ManagerController::initializeViews() {
    ManagerView& managerView = ManagerView::getInstance();

    // Register DefaultScreenView and DefaultScreenController
    DefaultScreenView* defaultView = new DefaultScreenView();
    DefaultScreenController* defaultController = new DefaultScreenController(&managerView);

    managerView.registerView("Default", defaultView);
    registerController("Default", defaultController);

    // Register MediaFileView and MediaFileController
    MediaFileView* mediaFileView = new MediaFileView();
    MediaFileController* mediaFileController = new MediaFileController(&managerView);

    managerView.registerView("MediaFile", mediaFileView);
    registerController("MediaFile", mediaFileController);

    // Set the initial view
    managerView.setView("Default");
}

void ManagerController::run() {
    ManagerView& managerView = ManagerView::getInstance();
    while (true) {
        if (managerView.getView() == nullptr) {
            std::cerr << "Error: Current view is null!" << std::endl;
            break;
        }

        managerView.getView()->showMenu();
        int action = managerView.getView()->handleInput();

        std::string currentViewKey = managerView.getCurrentViewKey();
        auto it = controllers.find(currentViewKey);

        if (it != controllers.end() && it->second != nullptr) {
            it->second->handleAction(action);
        }
        else {
            std::cerr << "Error: No controller found for view: " << currentViewKey << std::endl;
        }
    }
}

