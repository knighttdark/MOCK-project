#ifndef MANAGER_CONTROLLER_H
#define MANAGER_CONTROLLER_H

#include <unordered_map>
#include <string>
#include "common/BaseController.h"
#include "view/ManagerView.h"
#include "model/ManagerModel.h"
//#include "controller/PlaylistController.h"
#include "view/PlaylistView.h"




/* Manage controllers and views */
class ManagerController {
private:
    static ManagerController* instance;
    unordered_map<string, BaseController*> controllers;
    ManagerView* managerView;
    ManagerModel* managerModel;
    ManagerController();
    
public:

    friend class MockManagerController;  // ✅ Cho phép mock truy cập
    friend class DefaultScreenControllerTest; // ✅ Cho phép test sửa instance

    ManagerController(const ManagerController&) = delete;
    ManagerController& operator=(const ManagerController&) = delete;

    static ManagerController& getInstance();

    virtual ManagerView* getManagerView();
    virtual ManagerModel* getManagerModel();

    virtual BaseController* getController(const string& key) const;
    void setController(const string& key, BaseController* controller);
    void registerController(const string& key, BaseController* controller);
    void initializeViews();
     void run(bool isTest = false);

        // ✅ Cho phép thay thế instance trong test
    static void setInstance(ManagerController* mockInstance) {
        instance = mockInstance;
    }
    virtual ~ManagerController();
};

#endif
