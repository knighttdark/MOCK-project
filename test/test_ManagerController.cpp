#include <gtest/gtest.h>
#include "controller/ManagerController.h"
#include "view/ManagerView.h"
#include <gmock/gmock.h>
TEST(ManagerControllerTest, InitializeViewsTest) {
    ManagerController& controller = ManagerController::getInstance();
    ManagerView& view = ManagerView::getInstance();

    // Gọi hàm initializeViews để khởi tạo các view và controller
    controller.initializeViews();

    // Kiểm tra view hiện tại là Default (vì nó được đặt làm mặc định)
    EXPECT_NE(view.getView(), nullptr);  // View hiện tại không được null
    EXPECT_EQ(view.getCurrentViewKey(), "Default");  // View hiện tại là "Default"

    // Kiểm tra các controller đã được đăng ký
    EXPECT_NE(controller.getController("Default"), nullptr);  // Controller "Default" phải tồn tại
    EXPECT_NE(controller.getController("MediaFile"), nullptr); // Controller "MediaFile" phải tồn tại
    EXPECT_NE(controller.getController("Playlist"), nullptr);  // Controller "Playlist" phải tồn tại
    EXPECT_NE(controller.getController("PlayingView"), nullptr); // Controller "PlayingView" phải tồn tại
}
TEST(ManagerControllerTest, NoControllerErrorTest) {
    ManagerController& controller = ManagerController::getInstance();
    ManagerView& view = ManagerView::getInstance();

    // Khởi tạo views và đặt view không tồn tại controller
    controller.initializeViews();
    view.setView("NonExistentView");

    // Chạy hàm run
    testing::internal::CaptureStderr();
    controller.run();
    std::string output = testing::internal::GetCapturedStderr();

    // Kiểm tra thông báo lỗi
    EXPECT_NE(output.find("Error: No controller found for view: NonExistentView"), std::string::npos);
}
TEST(ManagerControllerTest, NoViewErrorTest) {
    ManagerController& controller = ManagerController::getInstance();
    ManagerView& view = ManagerView::getInstance();

    // Đặt view hiện tại là null
    view.setView("");

    // Chạy hàm run
    testing::internal::CaptureStderr();
    controller.run();
    std::string output = testing::internal::GetCapturedStderr();

    // Kiểm tra thông báo lỗi
    EXPECT_NE(output.find("Error: Current view is null!"), std::string::npos);
}



// exception
TEST(ManagerControllerTest, ConstructorCatchException) {
    // Giả lập ngoại lệ trong loadFromFile
    ManagerModel& model = ManagerModel::getInstance();
    EXPECT_THROW({
        throw std::runtime_error("Test exception");
    }, std::runtime_error);

    testing::internal::CaptureStderr();
    ManagerController& controller = ManagerController::getInstance();
    std::string output = testing::internal::GetCapturedStderr();

    EXPECT_NE(output.find("Error loading playlists: Test exception"), std::string::npos);
}

// //return null


class MockView : public BaseView {
public:
    MOCK_METHOD(int, showMenu, (), (override));
};

class MockController : public BaseController {
public:
    MOCK_METHOD(void, handleAction, (int action), (override));
};

TEST(ManagerControllerTest, RunLogicTest) {
    ManagerController& controller = ManagerController::getInstance();
    ManagerView& view = ManagerView::getInstance();

    // Tạo Mock cho View và Controller
    MockView* mockView = new MockView();
    MockController* mockController = new MockController();

    // Đăng ký Mock View và Controller
    view.registerView("MockView", mockView);
    controller.registerController("MockView", mockController);
    view.setView("MockView");

    // Giả lập input từ Mock View
    EXPECT_CALL(*mockView, showMenu())
        .Times(1)
        .WillOnce(::testing::Return(1));  // Trả về action = 1

    // Giả lập hành vi của Mock Controller khi nhận action = 1
    EXPECT_CALL(*mockController, handleAction(1))
        .Times(1);

    // Chạy logic chính
    controller.run();
}

// TEST(ManagerControllerTest, SetAndGetController) {
//     ManagerController& controller = ManagerController::getInstance();

//     // Tạo mock controller
//     MockController* mockController = new MockController();
//     controller.setController("MockController", mockController);

//     // Kiểm tra controller đã được đặt
//     EXPECT_EQ(controller.getController("MockController"), mockController);

//     // Kiểm tra trường hợp không tồn tại controller
//     EXPECT_EQ(controller.getController("NonExistentController"), nullptr);
// }

// TEST(ManagerControllerTest, DestructorTest) {
//     {
//         ManagerController& controller = ManagerController::getInstance();

//         // Tạo mock controller
//         MockController* mockController = new MockController();
//         controller.setController("MockController", mockController);

//         EXPECT_EQ(controller.getController("MockController"), mockController);
//     } // Kiểm tra tự động giải phóng trong destructor
//     // Không có crash nghĩa là destructor hoạt động đúng
// }
TEST(ManagerControllerTest, GetManagerInstances) {
    // Lấy singleton instance của ManagerController
    ManagerController& controller = ManagerController::getInstance();

    // Kiểm tra ManagerView
    ManagerView* view = controller.getManagerView();
    EXPECT_NE(view, nullptr); // Đảm bảo view không phải nullptr
    EXPECT_EQ(view, &ManagerView::getInstance()); // Đảm bảo view trả về là instance của ManagerView

    // Kiểm tra ManagerModel
    ManagerModel* model = controller.getManagerModel();
    EXPECT_NE(model, nullptr); // Đảm bảo model không phải nullptr
    EXPECT_EQ(model, &ManagerModel::getInstance()); // Đảm bảo model trả về là instance của ManagerModel
}