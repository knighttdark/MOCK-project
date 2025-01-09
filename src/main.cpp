#include "controller/ManagerController.h"
#include "view/ManagerView.h"
#include "model/ManagerModel.h"

int main() {
    // Lấy các instance duy nhất của các lớp quản lý
    ManagerController& managerController = ManagerController::getInstance();
    ManagerView& managerView = ManagerView::getInstance();
    ManagerModel& managerModel = ManagerModel::getInstance();

    // Khởi tạo các view và controller
    managerController.initializeViews();

    // Chạy ứng dụng
    managerController.run();

    return 0;
}
