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
    unordered_map<string, BaseController*> controllers; /* Registered controllers */
    ManagerView* managerView; /* Manager view */
    ManagerModel* managerModel; /* Manager model */
    ManagerController(); /* Private constructor */

public:
    ManagerController(const ManagerController&) = delete; /* Delete copy constructor */
    ManagerController& operator=(const ManagerController&) = delete; /* Delete assignment operator */

    static ManagerController& getInstance(); /* Get singleton instance */

    ManagerView* getManagerView(); /* Get manager view */
    ManagerModel* getManagerModel(); /* Get manager model */

    BaseController* getController(const string& key) const; /* Get a controller by key */
    void setController(const string& key, BaseController* controller); /* Set a controller */

    void registerController(const string& key, BaseController* controller); /* Register a controller */
    void initializeViews(); /* Initialize views */
    void run(); /* Run application */
    ~ManagerController(); /* Destructor */
};

#endif /* MANAGER_CONTROLLER_H */
