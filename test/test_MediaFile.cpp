// #include "gtest/gtest.h"
// #include "controller/MediaFileController.h"
// #include <filesystem>
// #include  "common/Enum.h"
// #include "model/MediaFile.h"

// // Test trường hợp quét thư mục hợp lệ
// TEST(MediaFileControllerTest, ScanDirectory_Success) {
//     MediaFileController controller;

//     std::string validPath = "/tmp/testdir"; // Đảm bảo tạo thư mục này trước test
//     std::filesystem::create_directory(validPath);

//     EXPECT_NO_THROW(controller.scanDirectory(validPath));

//     std::filesystem::remove(validPath); // Dọn dẹp sau test
// }

// // Test trường hợp quét thư mục không tồn tại
// TEST(MediaFileControllerTest, ScanDirectory_InvalidPath) {
//     MediaFileController controller;
//     std::string invalidPath = "/nonexistent/path";

//     try {
//         controller.scanDirectory(invalidPath);
//     } catch (const std::exception& e) {
//         SUCCEED() << "Handled error: " << e.what();  // Đảm bảo bài test không fail
//         return;
//     }

//     SUCCEED(); // Nếu không có lỗi, vẫn coi như bài test thành công
// }

// TEST(MediaFileControllerTest, HandleActionScan_ScanDirectory) {
//     MediaFileController controller;
//     int scanOption = SCAN_DIRECTORY;

//     EXPECT_NO_THROW(controller.handleActionScan(scanOption));
// }

// TEST(MediaFileControllerTest, HandleActionScan_ScanUSB_NoUSBDevices) {
//     MediaFileController controller;
//     int scanOption = SCAN_USB;

//     // Đảm bảo thư mục USB gốc không chứa gì
//     std::string usbRootPath = "/media/" + std::string(getenv("USER"));
//     std::filesystem::remove_all(usbRootPath);

//     EXPECT_NO_THROW(controller.handleActionScan(scanOption));
// }

// TEST(MediaFileControllerTest, ScanAndDisplayMedia) {
//     MediaFileController controller;

//     EXPECT_NO_THROW(controller.scanAndDisplayMedia());
// }

// TEST(MediaFileControllerTest, NextPage) {
//     MediaFileController controller;

//     // Giả sử ban đầu ở trang đầu tiên
//     EXPECT_NO_THROW(controller.nextPage());
// }

// TEST(MediaFileControllerTest, PreviousPage) {
//     MediaFileController controller;

//     // Giả sử ban đầu ở trang đầu tiên
//     EXPECT_NO_THROW(controller.previousPage());
// }

// TEST(MediaFileControllerTest, HandleAction_InvalidAction) {
//     MediaFileController controller;
//     int invalidAction = -1;

//     EXPECT_NO_THROW(controller.handleAction(invalidAction));
// }

// TEST(MediaFileControllerTest, HandleAction_ShowProperties) {
//     MediaFileController controller;
//     int action = ACTION_SHOW_PROPERTIES;

//     EXPECT_NO_THROW(controller.handleAction(action));
// }

// TEST(MediaFileControllerTest, HandleAction_PlayMedia) {
//     MediaFileController controller;
//     int action = ACTION_PLAY_MEDIA;

//     EXPECT_NO_THROW(controller.handleAction(action));
// }

#include "common/Enum.h"
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "controller/MediaFileController.h"
#include "view/MediaFileView.h"
#include "controller/ManagerController.h"
#include "model/MediaLibrary.h"

using ::testing::Return;
using ::testing::NiceMock;
using ::testing::_;

// Mock classes
class MockMediaFileView : public MediaFileView {
public:
    MOCK_METHOD(int, showOptionScan, (), ());
    MOCK_METHOD(std::string, promptDirectoryInput, (), ());
    MOCK_METHOD(void, displayMediaFiles, (const std::vector<std::string>&, int, const std::string&), ());
    MOCK_METHOD(int, getSelectedMediaID, (), ());
};

class MockManagerController : public ManagerController {
public:
    MOCK_METHOD(ManagerView*, getManagerView, (), ());
    MOCK_METHOD(BaseController*, getController, (const std::string&), ());
    MOCK_METHOD(ManagerModel*, getManagerModel, (), ());
};

class MockManagerView : public ManagerView {
public:
    MOCK_METHOD(BaseView*, getView, (), ());
    MOCK_METHOD(void, setView, (const std::string&), ());
};

class MockMediaLibrary : public MediaLibrary {
public:
    MOCK_METHOD(void, scanDirectory, (const std::string&), ());
    MOCK_METHOD(std::vector<MediaFile>, getMediaFilesForPage, (int, int), ());
    MOCK_METHOD(int, getTotalPages, (int), ());
    MOCK_METHOD(std::vector<MediaFile>, getMediaFiles, (), ());
};

// Test fixture class
class MediaFileControllerTest : public ::testing::Test {
protected:
    NiceMock<MockMediaFileView> mockMediaFileView;
    NiceMock<MockMediaLibrary> mockMediaLibrary;
    MediaFileController mediaFileController;
};

// Test switch cases
TEST_F(MediaFileControllerTest, HandleAction_ShowProperties) {
    EXPECT_CALL(mockMediaFileView, getSelectedMediaID()).WillOnce(Return(1));
    EXPECT_NO_THROW(mediaFileController.handleAction(ACTION_SHOW_PROPERTIES));
}

TEST_F(MediaFileControllerTest, HandleAction_PlayMedia) {
    EXPECT_CALL(mockMediaFileView, getSelectedMediaID()).WillOnce(Return(1));
    EXPECT_NO_THROW(mediaFileController.handleAction(ACTION_PLAY_MEDIA));
}

TEST_F(MediaFileControllerTest, HandleAction_AddToPlaylist) {
    EXPECT_CALL(mockMediaFileView, getSelectedMediaID()).WillOnce(Return(1));
    EXPECT_NO_THROW(mediaFileController.handleAction(ACTION_ADD_TO_PLAYLIST));
}

TEST_F(MediaFileControllerTest, HandleAction_ReturnToPlaying) {
    EXPECT_NO_THROW(mediaFileController.handleAction(ACTION_RETURN_TO_PLAYING));
}

TEST_F(MediaFileControllerTest, HandleAction_NextPage) {
    EXPECT_NO_THROW(mediaFileController.handleAction(ACTION_NEXT_PAGE));
}

TEST_F(MediaFileControllerTest, HandleAction_PreviousPage) {
    EXPECT_NO_THROW(mediaFileController.handleAction(ACTION_PREVIOUS_PAGE));
}
