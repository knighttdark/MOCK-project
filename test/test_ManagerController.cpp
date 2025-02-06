// #include <gtest/gtest.h>
// #include "controller/ManagerController.h"
// #include "controller/DefaultScreenController.h"
// #include "view/DefaultScreenView.h"
// #include "view/ManagerView.h"
// #include <gmock/gmock.h>
// TEST(ManagerControllerTest, InitializeViewsTest) {
//     ManagerController& controller = ManagerController::getInstance();
//     ManagerView& view = ManagerView::getInstance();

    
//     controller.initializeViews();


//     EXPECT_NE(view.getView(), nullptr); 
//     EXPECT_EQ(view.getCurrentViewKey(), "Default");  

//     // Kiểm tra các controller đã được đăng ký
//     EXPECT_NE(controller.getController("Default"), nullptr);  
//     EXPECT_NE(controller.getController("MediaFile"), nullptr); 
//     EXPECT_NE(controller.getController("Playlist"), nullptr);  
//     EXPECT_NE(controller.getController("PlayingView"), nullptr); 
// }

// // TEST(ManagerControllerTest, NoViewErrorTest) {
// //     ManagerController& controller = ManagerController::getInstance();
// //     ManagerView& view = ManagerView::getInstance();

// //     // Đặt view hiện tại là null
// //     view.setView("");

// //     // Chạy hàm run
// //     testing::internal::CaptureStderr();
// //     controller.run();
// //     std::string output = testing::internal::GetCapturedStderr();

// //     // Kiểm tra thông báo lỗi
// //     EXPECT_NE(output.find("Error: Current view is null!"), std::string::npos);
// // }




// class MockView : public BaseView {
// public:
//     MOCK_METHOD(int, showMenu, (), (override));
// };

// class MockController : public BaseController {
// public:
//     MOCK_METHOD(void, handleAction, (int action), (override));
// };

// // TEST(ManagerControllerTest, RunLogicTest) {
// //     ManagerController& controller = ManagerController::getInstance();
// //     ManagerView& view = ManagerView::getInstance();

// //     // Tạo Mock cho View và Controller
// //     MockView* mockView = new MockView();
// //     MockController* mockController = new MockController();

// //     // Đăng ký Mock View và Controller
// //     view.registerView("MockView", mockView);
// //     controller.registerController("MockView", mockController);
// //     view.setView("MockView");

// //     // Giả lập input từ Mock View
// //     EXPECT_CALL(*mockView, showMenu())
// //         .Times(1)
// //         .WillOnce(::testing::Return(1));  // Trả về action = 1

// //     // Giả lập hành vi của Mock Controller khi nhận action = 1
// //     EXPECT_CALL(*mockController, handleAction(1))
// //         .Times(1);

// //     // Chạy logic chính
// //     controller.run();
// // }

// //destructor
// TEST(ManagerControllerTest, DestructorClearsControllers) {
//     ManagerController& controller = ManagerController::getInstance();
//     controller.initializeViews();

//     EXPECT_NE(controller.getController("Default"), nullptr);

//     controller.~ManagerController();  // Gọi Destructor thủ công

//     EXPECT_EQ(controller.getController("Default"), nullptr);
// }
// //override controller
// TEST(ManagerControllerTest, RegisterControllerOverride) {
//     ManagerController& controller = ManagerController::getInstance();
//     controller.initializeViews();

//     BaseController* oldController = controller.getController("Default");
//     BaseController* newController = new DefaultScreenController();

//     controller.registerController("Default", newController);

//     EXPECT_NE(controller.getController("Default"), oldController);
//     EXPECT_EQ(controller.getController("Default"), newController);
// }

// //key not exist
// TEST(ManagerControllerTest, GetNonExistentController) {
//     ManagerController& controller = ManagerController::getInstance();
//     EXPECT_EQ(controller.getController("NonExistent"), nullptr);
// }

// // exception
// TEST(ManagerControllerTest, ConstructorCatchException) {
//     // Giả lập ngoại lệ trong loadFromFile
//     ManagerModel& model = ManagerModel::getInstance();
//     EXPECT_THROW({
//         throw std::runtime_error("Test exception");
//     }, std::runtime_error);

//     testing::internal::CaptureStderr();
//     ManagerController& controller = ManagerController::getInstance();
//     std::string output = testing::internal::GetCapturedStderr();

//     EXPECT_NE(output.find("Error loading playlists: Test exception"), std::string::npos);
// }

// TEST(ManagerControllerTest, GetManagerInstances) {
//     // Lấy singleton instance của ManagerController
//     ManagerController& controller = ManagerController::getInstance();

//     // Kiểm tra ManagerView
//     ManagerView* view = controller.getManagerView();
//     EXPECT_NE(view, nullptr); // Đảm bảo view không phải nullptr
//     EXPECT_EQ(view, &ManagerView::getInstance()); // Đảm bảo view trả về là instance của ManagerView

//     // Kiểm tra ManagerModel
//     ManagerModel* model = controller.getManagerModel();
//     EXPECT_NE(model, nullptr); // Đảm bảo model không phải nullptr
//     EXPECT_EQ(model, &ManagerModel::getInstance()); // Đảm bảo model trả về là instance của ManagerModel
// }
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "controller/ManagerController.h"
#include "view/ManagerView.h"
#include "common/BaseView.h"
#include "common/BaseController.h"
#include "controller/DefaultScreenController.h"

using ::testing::Return;


// Mock View để test mà không cần chạy UI thực tế
class MockView : public BaseView {
public:
    MOCK_METHOD(int, showMenu, (), (override));
};

// Mock Controller để kiểm tra handleAction
class MockController : public BaseController {
public:
    MOCK_METHOD(void, handleAction, (int action), (override));
};

// ✅ **1. Test initializeViews()**
TEST(ManagerControllerTest, InitializeViews) {
    ManagerController& controller = ManagerController::getInstance();
    controller.initializeViews();

    EXPECT_NE(controller.getController("Default"), nullptr);
    EXPECT_NE(controller.getController("MediaFile"), nullptr);
    EXPECT_NE(controller.getController("Playlist"), nullptr);
    EXPECT_NE(controller.getController("PlayingView"), nullptr);
}

// ✅ **2. Test getController()**
TEST(ManagerControllerTest, GetController) {
    ManagerController& controller = ManagerController::getInstance();
    controller.initializeViews();

    EXPECT_NE(controller.getController("Default"), nullptr);
    EXPECT_EQ(controller.getController("NonExistent"), nullptr);
}

// ✅ **3. Test registerController()**
// TEST(ManagerControllerTest, RegisterController) {
//     ManagerController& controller = ManagerController::getInstance();
//     controller.initializeViews();

//     BaseController* oldController = controller.getController("Default");
//     BaseController* newController = new DefaultScreenController();

//     controller.registerController("Default", newController);

//     EXPECT_NE(controller.getController("Default"), oldController);
//     EXPECT_EQ(controller.getController("Default"), newController);

//     delete newController; // Cleanup
// }

//✅ **4. Test handleAction() với Google Mock**
// TEST(ManagerControllerTest, HandleActionTest) {
//     ManagerController& controller = ManagerController::getInstance();
//     controller.initializeViews();

//     MockController* mockController = new MockController();
//     controller.registerController("MockView", mockController);

//     EXPECT_CALL(*mockController, handleAction(42)).Times(1); // Gọi đúng 1 lần với action = 42

//     controller.getController("MockView")->handleAction(42);

//     delete mockController;
// }

// ✅ **5. Test `run()` với Mock (tự động, không cần người nhập)**
// TEST(ManagerControllerTest, RunTestWithMockView_Auto) {
//     ManagerController& controller = ManagerController::getInstance();
//     ManagerView& view = ManagerView::getInstance();

//     MockView* mockView = new MockView();
//     view.registerView("MockView", mockView);
//     view.setView("MockView");

//     MockController* mockController = new MockController();
//     controller.registerController("MockView", mockController);

//     // 🚀 Giả lập input từ Mock View: dừng sau 4 lần gọi showMenu()
//     EXPECT_CALL(*mockView, showMenu())
//         .Times(4)  // Chỉ mong đợi 4 lần
//         .WillOnce(Return(1))
//         .WillOnce(Return(2))
//         .WillOnce(Return(3))
//         .WillOnce(Return(-1)) // Dừng vòng lặp
//         .WillRepeatedly(Return(-1)); // 🚀 Nếu bị gọi quá số lần mong đợi, luôn trả về -1

//     // 🚀 Giả lập hành vi của Mock Controller khi nhận action
//     EXPECT_CALL(*mockController, handleAction(1)).Times(1);
//     EXPECT_CALL(*mockController, handleAction(2)).Times(1);
//     EXPECT_CALL(*mockController, handleAction(3)).Times(1);
//     EXPECT_CALL(*mockController, handleAction(0)).Times(::testing::AtMost(1)); // Ignore action = 0 nếu có

//     controller.run();  // Chạy `run()` nhưng sẽ tự dừng

//     delete mockView;
//     delete mockController;
// }
// ✅ **5. Test `run()` với Mock (tự động, không cần người nhập)**
// TEST(ManagerControllerTest, RunTestWithMockView_Auto) {
//     ManagerController& controller = ManagerController::getInstance();
//     ManagerView& view = ManagerView::getInstance();

//     MockView* mockView = new MockView();
//     view.registerView("MockView", mockView);
//     view.setView("MockView");

//     MockController* mockController = new MockController();
//     controller.registerController("MockView", mockController);

//     // 🚀 Giả lập input từ Mock View để dừng sau 5 lần gọi
//     EXPECT_CALL(*mockView, showMenu())
//         .Times(5)
//         .WillOnce(Return(1))
//         .WillOnce(Return(2))
//         .WillOnce(Return(3))
//         .WillOnce(Return(4))
//         .WillOnce(Return(-1));  // Lần cuối cùng trả về -1 để thoát

//     // 🚀 Giả lập hành vi của Mock Controller khi nhận action
//     EXPECT_CALL(*mockController, handleAction(1)).Times(1);
//     EXPECT_CALL(*mockController, handleAction(2)).Times(1);
//     EXPECT_CALL(*mockController, handleAction(3)).Times(1);
//     EXPECT_CALL(*mockController, handleAction(4)).Times(1);
//     EXPECT_CALL(*mockController, handleAction(0)).Times(0);  // 🚀 Không cho phép gọi action(0)

//     controller.run(true);  // 🚀 Chạy `run(true)` để bật chế độ test

//     delete mockView;
//     delete mockController;
// }

// // ✅ **6. Test khi không có View được thiết lập**
// TEST(ManagerControllerTest, NoViewErrorTest) {
//     ManagerController& controller = ManagerController::getInstance();
//     ManagerView& view = ManagerView::getInstance();

//     // Đặt View hiện tại thành null
//     view.setView("");

//     testing::internal::CaptureStderr();
//     controller.run();
//     std::string output = testing::internal::GetCapturedStderr();

//     // Kiểm tra thông báo lỗi
//     EXPECT_NE(output.find("Error: No controller found for view"), std::string::npos);
// }

// ✅ **7. Test Destructor của ManagerController**
// TEST(ManagerControllerTest, DestructorClearsControllers) {
//     ManagerController& controller = ManagerController::getInstance();
//     controller.initializeViews();

//     EXPECT_NE(controller.getController("Default"), nullptr);

//     controller.~ManagerController();  // Gọi Destructor thủ công

//     EXPECT_EQ(controller.getController("Default"), nullptr);
// }

// ✅ **8. Test lấy instance của ManagerView & ManagerModel**
TEST(ManagerControllerTest, GetManagerInstances) {
    ManagerController& controller = ManagerController::getInstance();

    // Kiểm tra ManagerView
    ManagerView* view = controller.getManagerView();
    EXPECT_NE(view, nullptr);
    EXPECT_EQ(view, &ManagerView::getInstance());

    // Kiểm tra ManagerModel
    ManagerModel* model = controller.getManagerModel();
    EXPECT_NE(model, nullptr);
    EXPECT_EQ(model, &ManagerModel::getInstance());
}
