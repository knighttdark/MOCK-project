
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "controller/ManagerController.h"
#include "view/ManagerView.h"
#include "common/BaseView.h"
#include "common/BaseController.h"
#include "controller/DefaultScreenController.h"

using ::testing::Return;
using ::testing::Invoke;

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

// class TestManagerView : public ManagerView {
// public:
//     void setTestView(BaseView* testView) {
//         this->currentView = testView;
//     }
//     void setTestCurrentViewKey(const std::string& key) {
//         this->currentViewKey = key;
//     }
    
//     // Optionally override getView() and getCurrentViewKey() if needed.
// };
// TEST(ManagerControllerTest, RunTestLogic) {
//     // Get singleton instances
//     ManagerController& controller = ManagerController::getInstance();
//     ManagerView& managerView = ManagerView::getInstance();

//     // Create our mocks
//     MockView mockView;
//     MockController mockController;

//     // For testing we assume ManagerView allows injecting a custom view and current view key.
//     // (You might need to add these setters to ManagerView if they do not exist.)
//     managerView.setView(&mockView);                 // Inject our mock view into the ManagerView
//     std::string testViewKey = "TestView";
//     managerView.setCurrentViewKey(testViewKey);       // Set the current view key for testing

//     // Register our mock controller for this view key.
//     // (Assuming registerController is a method that adds the controller to ManagerController’s map.)
//     controller.registerController(testViewKey, &mockController);

//     // Set expectations on the mock view:
//     //  - First call to showMenu() returns an action (e.g. 42)
//     //  - Second call to showMenu() will throw an exception to break out of the infinite loop.
//     EXPECT_CALL(mockView, showMenu())
//         .WillOnce(Return(42))
//         .WillOnce(Invoke([]() -> int {
//             throw std::runtime_error("Test exit");
//         }));

//     // Expect that the mock controller’s handleAction() is called with action 42.
//     EXPECT_CALL(mockController, handleAction(42))
//         .Times(1);

//     // Run the ManagerController::run() method.
//     // Since the infinite loop is broken by our exception on the second iteration,
//     // we expect the run() method to eventually throw.
//     EXPECT_THROW({
//         controller.run(true);
//     }, std::runtime_error);
// }

// //✅ **5. Test `run()` với Mock (tự động, không cần người nhập)**
// TEST(ManagerControllerTest, RunTestHandlesViewAndAction) {
//     // ✅ Tạo mock cho ManagerController và ManagerView
//     ManagerController& controller = ManagerController::getInstance();
//     ManagerView& view = ManagerView::getInstance();

//     // ✅ Mock view để kiểm tra xem `showMenu()` có được gọi không
//     MockView* mockView = new MockView();
//     view.registerView("MockView", mockView);
//     view.setView("MockView");

//     // ✅ Mock controller để kiểm tra xem `handleAction()` có được gọi không
//     MockController* mockController = new MockController();
//     controller.registerController("MockView", mockController);

//     // ✅ Giả lập `showMenu()` trả về giá trị bất kỳ, sau đó trả về `-1` để dừng `run()`
//     EXPECT_CALL(*mockView, showMenu())
//         .Times(2)  // Gọi 2 lần: lần 1 trả về action hợp lệ, lần 2 trả về -1 để thoát
//         .WillOnce(Return(42))  // Giá trị 42 không quan trọng, chỉ cần khác -1
//         .WillOnce(Return(-1)); // Lần 2 trả về -1 để thoát khỏi vòng lặp

//     // ✅ Kiểm tra xem `handleAction()` có được gọi không
//     EXPECT_CALL(*mockController, handleAction(42)).Times(1);  // `run()` gọi `handleAction(42)`

//     // ✅ Chạy `run()`, nhưng test mode (`isTest = true`) để giới hạn số lần lặp
//     controller.run(true);

//     // ✅ Giải phóng mock sau khi test
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
