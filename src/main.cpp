#include "controller/ManagerController.h"
#include "view/ManagerView.h"
#include "model/ManagerModel.h"
#include "hardware/HardwareController.h"
#include <thread>


void hardwareThreadFunction() {
    try {
        // Initialize hardware with UART configuration
        Hardware hardware("/dev/ttyACM0", 115200);

        // Start listening for incoming UART messages
        hardware.startListening();
    } catch (exception& e) {
        cerr << "Error in hardware thread: " << e.what() << endl;
    }
}

void hardwareSendFunction() {
    try {
        // Initialize hardware with UART configuration
        Hardware hardware("/dev/ttyACM0", 115200);

        int minutes = 0;
        int seconds = 0;

        while (true) {
            // Create MM:SS formatted string
            ostringstream oss;
            oss << setw(2) << setfill('0') << minutes << ":"
                << setw(2) << setfill('0') << seconds;
            string message = oss.str() + "\r";

            // Send message via UART
            hardware.sendCommandToBoard(message);

            // Increment time
            this_thread::sleep_for(std::chrono::seconds(1));
            seconds++;
            if (seconds == 60) {
                seconds = 0;
                minutes++;
            }
        }
    } catch (exception& e) {
        cerr << "Error in hardware send thread: " << e.what() << endl;
    }
}

int main() {
    ManagerController& managerController = ManagerController::getInstance(); 
    ManagerView& managerView = ManagerView::getInstance(); 
    ManagerModel& managerModel = ManagerModel::getInstance(); 

    thread hardwareListenerThread(hardwareThreadFunction);
    thread hardwareSenderThread(hardwareSendFunction);

    managerController.initializeViews(); 
    managerController.run(); 

    
    if (hardwareListenerThread.joinable()) {
        hardwareListenerThread.join();
    }
    if (hardwareSenderThread.joinable()) {
        hardwareSenderThread.join();
    }

    return 0;
}
