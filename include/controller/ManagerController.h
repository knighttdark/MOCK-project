#ifndef MANAGER_CONTROLLER_H
#define MANAGER_CONTROLLER_H

#include <unordered_map>
#include <string>
#include "common/BaseController.h"
#include "view/ManagerView.h"
#include "model/ManagerModel.h"
#include "controller/PlaylistController.h"
#include "view/PlaylistView.h"




/* Manage controllers and views */
class ManagerController {
private:
    unordered_map<string, BaseController*> controllers;
    ManagerView* managerView;
    ManagerModel* managerModel;
    ManagerController();

public:
    ManagerController(const ManagerController&) = delete;
    ManagerController& operator=(const ManagerController&) = delete;

    static ManagerController& getInstance();

    ManagerView* getManagerView();
    ManagerModel* getManagerModel();

    BaseController* getController(const string& key) const;
    void setController(const string& key, BaseController* controller);
    void registerController(const string& key, BaseController* controller);
    void initializeViews();
    void run();

    ~ManagerController();
};

#endif
