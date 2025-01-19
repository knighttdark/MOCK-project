#include "controller/ManagerController.h"
#include "view/ManagerView.h"
#include "model/ManagerModel.h"

/* Main entry point of the application */
int main() {
    ManagerController& managerController = ManagerController::getInstance(); /* Get ManagerController instance */
    managerController.initializeViews(); /* Initialize views */
    managerController.run(); /* Run application */

    return 0;
}
