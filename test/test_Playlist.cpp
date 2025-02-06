#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "controller/PlaylistController.h"
#include "controller/ManagerController.h"
#include "model/ManagerModel.h"
#include "view/PlaylistView.h"
#include "controller/PlayingMediaController.h"

using ::testing::Return;
using ::testing::_;
using ::testing::NiceMock;

/* Mock PlaylistView */
class MockPlaylistView : public PlaylistView {
public:
    MOCK_METHOD(int, getSelectedPlaylistID, (), (const));
    MOCK_METHOD(void, displayPlaylistDetails, (const Playlist&));  
    MOCK_METHOD(void, displayPlaylists, (const std::vector<Playlist>&));  
};

/* Mock PlayingMediaController */
class MockPlayingMediaController : public PlayingMediaController {
public:
    MOCK_METHOD(void, playPlaylist, (std::vector<MediaFile>&));  
    MOCK_METHOD(void, stop, ());
};

/* Class test */
class PlaylistControllerTest : public ::testing::Test {
protected:
    PlaylistController controller;
    ManagerController& managerController = ManagerController::getInstance();
    ManagerModel& managerModel = ManagerModel::getInstance();
    NiceMock<MockPlaylistView> mockPlaylistView;
    NiceMock<MockPlayingMediaController> mockPlayingMediaController;

    void SetUp() override {
        managerModel.getPlaylistLibrary().getPlaylists().clear();

        // Đăng ký mock view và controller
        managerController.getManagerView()->registerView("PlaylistView", &mockPlaylistView);
        EXPECT_CALL(mockPlaylistView, getSelectedPlaylistID()).WillRepeatedly(Return(1));
    }
};

/* ✅ Kiểm tra `handleAction(ACTION_CREATE_PLAYLIST)` */
TEST_F(PlaylistControllerTest, HandleAction_CreatePlaylist) {
    testing::internal::CaptureStdout();
    controller.handleAction(ACTION_CREATE_PLAYLIST);
    std::string output = testing::internal::GetCapturedStdout();

    // Kiểm tra kết quả đầu ra
    EXPECT_NE(output.find("Playlist 'New Playlist' created successfully."), std::string::npos);

    // Kiểm tra playlist thực sự đã được tạo
    PlaylistLibrary& playlistLibrary = managerModel.getPlaylistLibrary();
    EXPECT_EQ(playlistLibrary.getPlaylists().size(), 1);
}

/* ✅ Kiểm tra `handleAction(ACTION_LIST_ALL_PLAYLISTS)` */
TEST_F(PlaylistControllerTest, HandleAction_ListAllPlaylists) {
    testing::internal::CaptureStdout();
    controller.handleAction(ACTION_LIST_ALL_PLAYLISTS);
    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_NE(output.find("No playlists available."), std::string::npos);
}

/* ✅ Kiểm tra `createPlaylist()` khi nhập tên rỗng */
TEST_F(PlaylistControllerTest, CreatePlaylist_Fail_EmptyName) {
    testing::internal::CaptureStderr();
    controller.createPlaylist("");
    std::string output = testing::internal::GetCapturedStderr();
    EXPECT_NE(output.find("Error: Playlist name cannot be empty."), std::string::npos);
}

/* ✅ Kiểm tra `deletePlaylist()` khi `getSelectedPlaylistID()` trả về ID không hợp lệ */
TEST_F(PlaylistControllerTest, DeletePlaylist_Fail_InvalidID) {
    PlaylistLibrary& playlistLibrary = managerModel.getPlaylistLibrary();
    playlistLibrary.addPlaylist(Playlist("Playlist 1"));

    EXPECT_CALL(mockPlaylistView, getSelectedPlaylistID()).WillOnce(Return(5)); // ID không hợp lệ

    testing::internal::CaptureStderr();
    controller.deletePlaylist();
    std::string output = testing::internal::GetCapturedStderr();
    EXPECT_NE(output.find("Error: Invalid Playlist ID!"), std::string::npos);
}

/* ✅ Kiểm tra lỗi khi danh sách playlist rỗng mà gọi `deletePlaylist()` */
TEST_F(PlaylistControllerTest, DeletePlaylist_Fail_EmptyList) {
    testing::internal::CaptureStderr();
    controller.deletePlaylist();
    std::string output = testing::internal::GetCapturedStderr();
    EXPECT_NE(output.find("No playlists available to delete."), std::string::npos);
}

/* ✅ Kiểm tra lỗi khi không có `PlayingMediaController` trong `playPlaylist()` */
TEST_F(PlaylistControllerTest, PlayPlaylist_Fail_NoController) {
    PlaylistLibrary& playlistLibrary = managerModel.getPlaylistLibrary();
    Playlist playlist("My Playlist");
    playlist.addSong(MediaFile(1, "Song 1", "/path/to/song1.mp3", "mp3"));
    playlistLibrary.addPlaylist(playlist);

    // Không đăng ký PlayingMediaController
    testing::internal::CaptureStderr();
    controller.playPlaylist("My Playlist");
    std::string output = testing::internal::GetCapturedStderr();
    EXPECT_NE(output.find("Error: PlayingMediaController is not available."), std::string::npos);
}

/* ✅ Kiểm tra lỗi khi `PlaylistView` không tồn tại trong `viewPlaylistDetails()` */
TEST_F(PlaylistControllerTest, ViewPlaylistDetails_Fail_NoView) {
    managerController.getManagerView()->registerView("PlaylistView", nullptr);

    testing::internal::CaptureStderr();
    controller.viewPlaylistDetails("My Playlist");
    std::string output = testing::internal::GetCapturedStderr();
    EXPECT_NE(output.find("Error: PlaylistView is not available."), std::string::npos);
}

/* ✅ Kiểm tra `ACTION_EXIT_PLAYLIST_MENU` */
TEST_F(PlaylistControllerTest, HandleAction_ExitPlaylistMenu) {
    EXPECT_CALL(mockPlayingMediaController, stop()).Times(1);
    controller.handleAction(ACTION_EXIT_PLAYLIST_MENU);
    EXPECT_EQ(managerController.getManagerView()->getCurrentViewKey(), "Default");
}

/* ✅ Kiểm tra `playPlaylist()` với playlist hợp lệ */
TEST_F(PlaylistControllerTest, PlayPlaylist_Success) {
    PlaylistLibrary& playlistLibrary = managerModel.getPlaylistLibrary();
    Playlist playlist("My Playlist");
    playlist.addSong(MediaFile(1, "Song 1", "/path/to/song1.mp3", "mp3"));
    playlistLibrary.addPlaylist(playlist);

    EXPECT_CALL(mockPlayingMediaController, playPlaylist(_)).Times(1);
    controller.playPlaylist("My Playlist");
}

/* ✅ Kiểm tra lỗi khi `playPlaylist()` gọi với playlist không có bài hát */
TEST_F(PlaylistControllerTest, PlayPlaylist_Fail_NoSongs) {
    PlaylistLibrary& playlistLibrary = managerModel.getPlaylistLibrary();
    playlistLibrary.addPlaylist(Playlist("My Playlist"));

    testing::internal::CaptureStderr();
    controller.playPlaylist("My Playlist");
    std::string output = testing::internal::GetCapturedStderr();
    EXPECT_NE(output.find("No songs in playlist"), std::string::npos);
}
