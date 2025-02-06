#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "controller/DefaultScreenController.h"
#include "controller/ManagerController.h"
#include "controller/MediaFileController.h"
#include "controller/PlaylistController.h"
#include "controller/PlayingMediaController.h"
#include "view/ManagerView.h"
#include "view/MediaFileView.h"
#include "view/PlaylistView.h"

using namespace testing;

// ==========================
// Mock Classes
// ==========================
class MockManagerController : public ManagerController {
public:
    MOCK_METHOD(BaseController*, getController, (const std::string& key), (const, override));
    MOCK_METHOD(ManagerView*, getManagerView, (), (override));
};

class MockMediaFileController : public MediaFileController {
public:
    MOCK_METHOD(void, handleActionScan, (int option), (override));
    MOCK_METHOD(void, scanAndDisplayMedia, (), (override));
};

class MockPlaylistController : public PlaylistController {
public:
    MOCK_METHOD(void, listAllPlaylists, (), (override));
};

class MockPlayingMediaController : public PlayingMediaController {
public:
    MOCK_METHOD(void, stop, (), (override));
};

class MockMediaFileView : public MediaFileView {
public:
    MOCK_METHOD(int, showOptionScan, (), (override));
};

class MockPlaylistView : public PlaylistView {
public:
    MOCK_METHOD(void, displayPlaylists, (const vector<Playlist>& playlists), (override));
};

class MockManagerView : public ManagerView {
public:
    MOCK_METHOD(void, setView, (const std::string& key), (override));
    MOCK_METHOD(BaseView*, getView, (), (const, override));
    MOCK_METHOD(std::string, getCurrentViewKey, (), (const, override));
};

// ==========================
// Test Fixture for DefaultScreenController
// ==========================
class DefaultScreenControllerTest : public ::testing::Test {
protected:
    MockManagerController* mockManagerController;
    MockManagerView* mockManagerView;
    MockMediaFileController* mockMediaFileController;
    MockPlaylistController* mockPlaylistController;
    MockPlayingMediaController* mockPlayingMediaController;
    MockMediaFileView* mockMediaFileView;
    MockPlaylistView* mockPlaylistView;
    DefaultScreenController* controller;

void SetUp() override {
    mockManagerController = new MockManagerController();
    mockManagerView = new MockManagerView();
    mockMediaFileController = new MockMediaFileController();
    mockPlaylistController = new MockPlaylistController();
    mockPlayingMediaController = new MockPlayingMediaController();
    mockMediaFileView = new MockMediaFileView();
    mockPlaylistView = new MockPlaylistView();

    // ✅ Thay thế singleton thực bằng mock trong test
    ManagerController::setInstance(mockManagerController);
    ManagerView::setInstance(mockManagerView);

    // Mock ManagerController behavior
    ON_CALL(*mockManagerController, getManagerView())
        .WillByDefault(Return(mockManagerView));

    ON_CALL(*mockManagerController, getController("MediaFile"))
        .WillByDefault(Return(mockMediaFileController));
    ON_CALL(*mockManagerController, getController("Playlist"))
        .WillByDefault(Return(mockPlaylistController));
    ON_CALL(*mockManagerController, getController("PlayingView"))
        .WillByDefault(Return(mockPlayingMediaController));

    // Mock ManagerView behavior
    ON_CALL(*mockManagerView, getView())
        .WillByDefault(Return(mockPlaylistView));

    // ✅ Truyền mock vào DefaultScreenController
    controller = new DefaultScreenController(mockManagerController);
}

void TearDown() override {
    delete controller;
    delete mockManagerController;
    delete mockManagerView;
    delete mockMediaFileController;
    delete mockPlaylistController;
    delete mockPlayingMediaController;
    delete mockMediaFileView;
    delete mockPlaylistView;

    // ✅ Reset singleton để không ảnh hưởng test khác
    ManagerController::setInstance(nullptr);
    ManagerView::setInstance(nullptr);
}

};

// ==========================
// TEST CASE 1: Media File Switching (Full Path)
// ==========================
TEST_F(DefaultScreenControllerTest, HandleActionSwitchToMediaFile) {
    EXPECT_CALL(*mockManagerController, getController("MediaFile"))
        .WillOnce(Return(mockMediaFileController));

    EXPECT_CALL(*mockManagerController, getManagerView())
        .WillRepeatedly(Return(mockManagerView));

    EXPECT_CALL(*mockManagerView, setView("MediaFile"));

    EXPECT_CALL(*mockManagerView, getView())
        .WillOnce(Return(mockMediaFileView));

    EXPECT_CALL(*mockMediaFileView, showOptionScan()).WillOnce(Return(1));

    EXPECT_CALL(*mockMediaFileController, handleActionScan(1)).Times(1);
    EXPECT_CALL(*mockMediaFileController, scanAndDisplayMedia()).Times(1);

    controller->handleAction(ACTION_SWITCH_TO_MEDIA_FILE);
}

// ==========================
// TEST CASE 2: Playlist Switching (Full Path)
// ==========================
TEST_F(DefaultScreenControllerTest, HandleActionSwitchToPlaylist) {
    EXPECT_CALL(*mockManagerController, getController("Playlist"))
        .WillOnce(Return(mockPlaylistController));

    EXPECT_CALL(*mockManagerController, getManagerView())
        .WillRepeatedly(Return(mockManagerView));

    EXPECT_CALL(*mockManagerView, setView("Playlist"));

    EXPECT_CALL(*mockManagerView, getView())
        .WillOnce(Return(mockPlaylistView));

    EXPECT_CALL(*mockPlaylistController, listAllPlaylists()).Times(1);

    controller->handleAction(ACTION_SWITCH_TO_PLAYLIST);
}

// ==========================
// TEST CASE 3: Exit Application (Modified for Test Mode)
// ==========================
TEST_F(DefaultScreenControllerTest, HandleActionExitApplication) {
    EXPECT_CALL(*mockManagerController, getController("PlayingView"))
        .WillOnce(Return(mockPlayingMediaController));

    EXPECT_CALL(*mockPlayingMediaController, stop()).Times(1);

    // Không kiểm tra exit(0), chỉ kiểm tra hành vi stop()
    DefaultScreenController testController(mockManagerController, true);
    testController.handleAction(ACTION_EXIT_APPLICATION);
    //controller->handleAction(ACTION_EXIT_APPLICATION);

    EXPECT_TRUE(true);  // Test thành công nếu không bị crash
}

// ==========================
// TEST CASE 4: Invalid Action
// ==========================
TEST_F(DefaultScreenControllerTest, HandleActionInvalid) {
    testing::internal::CaptureStdout();

    controller->handleAction(-1);  // Invalid action

    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_NE(output.find("Invalid choice! Please try again."), std::string::npos);
}
