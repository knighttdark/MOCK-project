#ifndef MANAGER_CONTROLLER_H
#define MANAGER_CONTROLLER_H

#include <unordered_map>
#include <string>
#include "common/BaseController.h"
#include "view/ManagerView.h"
#include "model/ManagerModel.h"

class ManagerController {
private:
    std::unordered_map<std::string, BaseController*> controllers;

    // Private constructor for Singleton
    ManagerController();

public:
    // Delete copy constructor and assignment operator
    ManagerController(const ManagerController&) = delete;
    ManagerController& operator=(const ManagerController&) = delete;

    // Static method to get the instance
    static ManagerController& getInstance();

    void registerController(const std::string& key, BaseController* controller);
    void initializeViews();
    void run();
};

#endif // MANAGER_CONTROLLER_HPP
