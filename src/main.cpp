#include "controller/ManagerController.h"
#include "view/ManagerView.h"
#include "model/ManagerModel.h"

/* Main entry point of the application */
int main() {
    ManagerController& managerController = ManagerController::getInstance(); /* Get ManagerController instance */
    ManagerView& managerView = ManagerView::getInstance(); /* Get ManagerView instance */
    ManagerModel& managerModel = ManagerModel::getInstance(); /* Get ManagerModel instance */

    managerController.initializeViews(); /* Initialize views */
    managerController.run(); /* Run application */

    return 0;
}
