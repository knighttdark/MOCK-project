// #include "gtest/gtest.h"
// #include "controller/MediaFileController.h"
// #include <filesystem>
// #include "common/Enum.h"
// #include "model/MediaFile.h"

// // Test trường hợp quét thư mục hợp lệ
// TEST(MediaFileControllerTest, ScanDirectory_Success) {
//     MediaFileController controller;

//     std::string validPath = "/tmp/testdir"; // Đảm bảo tạo thư mục này trước test
//     std::filesystem::create_directory(validPath);

//     EXPECT_NO_THROW(controller.scanDirectory(validPath));

//     std::filesystem::remove_all(validPath); // Dọn dẹp sau test
// }

// // Test trường hợp quét thư mục không tồn tại
// TEST(MediaFileControllerTest, ScanDirectory_InvalidPath) {
//     MediaFileController controller;
//     std::string invalidPath = "/nonexistent/path";

//     EXPECT_NO_THROW(controller.scanDirectory(invalidPath));
// }

// TEST(MediaFileControllerTest, HandleActionScan_ScanDirectory) {
//     MediaFileController controller;
//     int scanOption = SCAN_DIRECTORY;

//     EXPECT_NO_THROW(controller.handleActionScan(scanOption));
// }

// TEST(MediaFileControllerTest, HandleActionScan_ScanUSB_NoUSBDevices) {
//     MediaFileController controller;
//     int scanOption = SCAN_USB;

//     // Giả lập trường hợp không có thiết bị USB nào
//     std::string usbRootPath = "/media/" + std::string(getenv("USER"));
    
//     if (std::filesystem::exists(usbRootPath)) {
//         try {
//             std::filesystem::remove_all(usbRootPath);
//         } catch (const std::exception& e) {
//             std::cerr << "Error removing USB root path: " << e.what() << std::endl;
//         }
//     }

//     EXPECT_NO_THROW(controller.handleActionScan(scanOption));
// }

// TEST(MediaFileControllerTest, ScanAndDisplayMedia) {
//     MediaFileController controller;

//     EXPECT_NO_THROW(controller.scanAndDisplayMedia());
// }

// TEST(MediaFileControllerTest, NextPage) {
//     MediaFileController controller;

//     EXPECT_NO_THROW(controller.nextPage());
// }

// TEST(MediaFileControllerTest, PreviousPage) {
//     MediaFileController controller;

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


// #include "common/Enum.h"
// #include <gtest/gtest.h>
// #include <gmock/gmock.h>
// #include "controller/MediaFileController.h"
// #include "view/MediaFileView.h"
// #include "controller/ManagerController.h"
// #include "model/MediaLibrary.h"

// using ::testing::Return;
// using ::testing::NiceMock;
// using ::testing::_;

// // Mock classes
// class MockMediaFileView : public MediaFileView {
// public:
//     MOCK_METHOD(int, showOptionScan, (), ());
//     MOCK_METHOD(std::string, promptDirectoryInput, (), ());
//     MOCK_METHOD(void, displayMediaFiles, (const std::vector<std::string>&, int, const std::string&), ());
//     MOCK_METHOD(int, getSelectedMediaID, (), ());
// };

// class MockManagerController : public ManagerController {
// public:
//     MOCK_METHOD(ManagerView*, getManagerView, (), ());
//     MOCK_METHOD(BaseController*, getController, (const std::string&), ());
//     MOCK_METHOD(ManagerModel*, getManagerModel, (), ());
// };

// class MockManagerView : public ManagerView {
// public:
//     MOCK_METHOD(BaseView*, getView, (), ());
//     MOCK_METHOD(void, setView, (const std::string&), ());
// };

// class MockMediaLibrary : public MediaLibrary {
// public:
//     MOCK_METHOD(void, scanDirectory, (const std::string&), ());
//     MOCK_METHOD(std::vector<MediaFile>, getMediaFilesForPage, (int, int), ());
//     MOCK_METHOD(int, getTotalPages, (int), ());
//     MOCK_METHOD(std::vector<MediaFile>, getMediaFiles, (), ());
// };

// // Test fixture class
// class MediaFileControllerTest : public ::testing::Test {
// protected:
//     NiceMock<MockMediaFileView> mockMediaFileView;
//     NiceMock<MockMediaLibrary> mockMediaLibrary;
//     MediaFileController mediaFileController;
// };

#include "controller/MediaFileController.h"
#include "view/MediaFileView.h"
#include "controller/ManagerController.h"
#include "model/MediaLibrary.h"
#include "common/Enum.h"
#include <gtest/gtest.h>
#include <gmock/gmock.h>

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

// Test case for scanDirectory
TEST_F(MediaFileControllerTest, ScanDirectory_Success) {
    EXPECT_CALL(mockMediaLibrary, scanDirectory("test_path")).Times(1);

    mediaFileController.scanDirectory("test_path");
}

// Test case for handleActionScan with SCAN_DIRECTORY option
TEST_F(MediaFileControllerTest, HandleActionScan_ScanDirectory) {
    EXPECT_CALL(mockMediaFileView, promptDirectoryInput()).WillOnce(Return("test_path"));
    EXPECT_CALL(mockMediaLibrary, scanDirectory("test_path")).Times(1);

    mediaFileController.handleActionScan(SCAN_DIRECTORY);
}

// Test case for scanAndDisplayMedia
TEST_F(MediaFileControllerTest, ScanAndDisplayMedia) {
    std::vector<MediaFile> mockFiles = {
        MediaFile(1, "file1.mp3", "/path/file1.mp3", "audio")
    };
    std::vector<std::string> expectedDisplayFiles = {"1. file1.mp3"};

    EXPECT_CALL(mockMediaLibrary, getMediaFilesForPage(0, _)).WillOnce(Return(mockFiles));
    EXPECT_CALL(mockMediaFileView, displayMediaFiles(expectedDisplayFiles, 1, "")).Times(1);

    mediaFileController.scanAndDisplayMedia();
}

// Test case for nextPage functionality
TEST_F(MediaFileControllerTest, NextPage) {
    EXPECT_CALL(mockMediaLibrary, getTotalPages(_)).WillOnce(Return(2));
    EXPECT_CALL(mockMediaLibrary, getMediaFilesForPage(1, _)).WillOnce(Return(std::vector<MediaFile>{}));

    mediaFileController.nextPage();
}

// Test case for previousPage functionality
TEST_F(MediaFileControllerTest, PreviousPage) {
    EXPECT_CALL(mockMediaLibrary, getMediaFilesForPage(0, _)).WillOnce(Return(std::vector<MediaFile>{}));

    mediaFileController.previousPage();
}

// Test case for getting media path by ID
TEST_F(MediaFileControllerTest, GetPathById_ValidID) {
    std::vector<MediaFile> mediaFiles = {
        MediaFile(1, "file1.mp3", "/path/to/file1.mp3", "audio"),
        MediaFile(2, "file2.mp3", "/path/to/file2.mp3", "audio")
    };

    EXPECT_EQ(mediaFileController.getPathById(mediaFiles, 2), "/path/to/file2.mp3");
}

// Test case for handling invalid media ID
TEST_F(MediaFileControllerTest, GetPathById_InvalidID) {
    std::vector<MediaFile> mediaFiles = {
        MediaFile(1, "file1.mp3", "/path/to/file1.mp3", "audio"),
        MediaFile(2, "file2.mp3", "/path/to/file2.mp3", "audio")
    };

    EXPECT_EQ(mediaFileController.getPathById(mediaFiles, 3), "");
}
