#include "controller/ManagerController.h"
#include "view/ManagerView.h"
#include "model/ManagerModel.h"
#include "hardware/HardwareController.h"
#include <thread>


void hardwareThreadFunction() {
    try {
        Hardware hardware("/dev/ttyACM0", 115200);

        hardware.startListening();
    }
    catch (exception& e) {
        cerr << "Error in hardware thread: " << e.what() << endl;
    }
}

int main() {
    ManagerController& managerController = ManagerController::getInstance();

    thread hardwareThread(hardwareThreadFunction);

    managerController.initializeViews();
    managerController.run(false);

    if (hardwareThread.joinable()) {
        hardwareThread.join();
    }

    return 0;
}
