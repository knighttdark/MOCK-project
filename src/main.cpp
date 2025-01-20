#include "controller/ManagerController.h"
#include "view/ManagerView.h"
#include "model/ManagerModel.h"
#include "hardware/HardwareController.h"
#include <thread>

/* Hàm chạy Hardware trong một luồng riêng */
void hardwareThreadFunction() {
    try {
        // Tạo đối tượng Hardware
        Hardware hardware("/dev/ttyACM0", 115200);

        // Bắt đầu lắng nghe lệnh từ cổng nối tiếp
        hardware.startListening();
    }
    catch (exception& e) {
        cerr << "Error in hardware thread: " << e.what() << endl;
    }
}

/* Main entry point of the application */
int main() {
    ManagerController& managerController = ManagerController::getInstance(); /* Get ManagerController instance */
    ManagerView& managerView = ManagerView::getInstance(); /* Get ManagerView instance */
    ManagerModel& managerModel = ManagerModel::getInstance(); /* Get ManagerModel instance */

    thread hardwareThread(hardwareThreadFunction);

    managerController.initializeViews(); /* Initialize views */
    managerController.run(); /* Run application */

    // Đợi luồng Hardware kết thúc (nếu cần thiết)
    if (hardwareThread.joinable()) {
        hardwareThread.join();
    }

    return 0;
}
