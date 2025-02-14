

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "controller/PlaylistController.h"
#include "controller/ManagerController.h"
#include "model/ManagerModel.h"
#include "view/PlaylistView.h"
#include "common/MockClass.h"
#include "controller/PlayingMediaController.h"

using namespace testing;

class PlaylistControllerTest : public ::testing::Test {
protected:
    std::unique_ptr<MockManagerController> mockManagerController;
    std::unique_ptr<MockManagerView> mockManagerView;
    std::unique_ptr<MockManagerModel> mockManagerModel;
    std::unique_ptr<MockPlaylistLibrary> mockPlaylistLibrary;
    std::unique_ptr<MockPlaylistView> mockPlaylistView;
    std::unique_ptr<MockPlayingMediaController> mockPlayingController;
    std::unique_ptr<MockPlaylistController> mockPlaylistController;  // Declare MockPlaylistController here
    std::unique_ptr<PlaylistController> playlistController;  // Declare PlaylistController here
    std::string username = std::getenv("USER");
    std::string mediaPath = "/home/" + username + "/test_media";

    void SetUp() override {
        mockManagerController = std::make_unique<MockManagerController>();
        mockManagerView = std::make_unique<MockManagerView>();
        mockManagerModel = std::make_unique<MockManagerModel>();
        mockPlaylistLibrary = std::make_unique<MockPlaylistLibrary>();
        mockPlaylistView = std::make_unique<MockPlaylistView>();
        mockPlayingController = std::make_unique<MockPlayingMediaController>();
        mockPlaylistController = std::make_unique<MockPlaylistController>();  // Initialize MockPlaylistController

        std::vector<Playlist> playlists;  // Danh sách trống (có thể thay đổi nếu cần)
        ON_CALL(*mockPlaylistLibrary, getPlaylists()).WillByDefault(ReturnRef(playlists));
        

        // Set up ManagerController instance
        ManagerController::setInstance(mockManagerController.get());
        ManagerView::setInstance(mockManagerView.get());
        ManagerModel::setInstance(mockManagerModel.get());

        // Mock methods
        EXPECT_CALL(*mockManagerController, getManagerView())
            .WillRepeatedly(Return(mockManagerView.get()));

        EXPECT_CALL(*mockManagerView, getView())
            .WillRepeatedly(Return(mockPlaylistView.get()));

        EXPECT_CALL(*mockManagerController, getManagerModel())
            .WillRepeatedly(Return(mockManagerModel.get()));

        EXPECT_CALL(*mockManagerModel, getPlaylistLibrary())
            .WillRepeatedly(ReturnRef(*mockPlaylistLibrary));

        EXPECT_CALL(*mockManagerController, getController("PlayingView"))
            .WillRepeatedly(Return(mockPlayingController.get()));

        // Create PlaylistController with mocked ManagerController
        playlistController = std::make_unique<PlaylistController>(mockManagerController.get());
    }

    void TearDown() override {
        ManagerController::setInstance(nullptr);
        ManagerView::setInstance(nullptr);
        ManagerModel::setInstance(nullptr);

        testing::Mock::VerifyAndClearExpectations(mockPlayingController.get());

        mockManagerController.reset();
        mockManagerView.reset();
        mockManagerModel.reset();
        mockPlaylistLibrary.reset();
        mockPlaylistView.reset();
        mockPlayingController.reset();
        mockPlaylistController.reset();  // Reset MockPlaylistController

    }
};


TEST_F(PlaylistControllerTest, CreatePlaylist_Success) {
    std::string playlistName = "Rock Classics";

    // Expect that getPlaylistByName returns nullptr (playlist does not exist)
    EXPECT_CALL(*mockPlaylistLibrary, getPlaylistByName(playlistName))
        .WillOnce(Return(nullptr));

    // Expect addPlaylist to be called
    EXPECT_CALL(*mockPlaylistLibrary, addPlaylist(_))
        .Times(1);

    // Expect saveToFile to be called
    EXPECT_CALL(*mockPlaylistLibrary, saveToFile("playlists.txt"))
        .Times(1);

    // Call the method under test
    playlistController->createPlaylist(playlistName);
}

TEST_F(PlaylistControllerTest, CreatePlaylist_DuplicatePlaylist) {
    std::string playlistName = "Rock Classics";
    Playlist existingPlaylist(playlistName);

    // Expect getPlaylistByName to return an existing playlist
    EXPECT_CALL(*mockPlaylistLibrary, getPlaylistByName(playlistName))
        .WillOnce(Return(&existingPlaylist));

    // Ensure addPlaylist and saveToFile are never called
    EXPECT_CALL(*mockPlaylistLibrary, addPlaylist(_)).Times(0);
    EXPECT_CALL(*mockPlaylistLibrary, saveToFile(_)).Times(0);

    // Call the method under test
    playlistController->createPlaylist(playlistName);
}

TEST_F(PlaylistControllerTest, CreatePlaylist_SaveToFileFails) {
    std::string playlistName = "Rock Classics";

    // Expect getPlaylistByName returns nullptr (playlist does not exist)
    EXPECT_CALL(*mockPlaylistLibrary, getPlaylistByName(playlistName))
        .WillOnce(Return(nullptr));

    // Expect addPlaylist to be called
    EXPECT_CALL(*mockPlaylistLibrary, addPlaylist(_))
        .Times(1);

    // Simulate an exception in saveToFile
    EXPECT_CALL(*mockPlaylistLibrary, saveToFile("playlists.txt"))
        .WillOnce(Throw(std::runtime_error("Disk write error")));

    // Call the method under test
    playlistController->createPlaylist(playlistName);
}

TEST_F(PlaylistControllerTest, DeletePlaylist_Success) {
    std::string playlistName = "Rock Classics";
    Playlist playlist(playlistName);
    std::vector<Playlist> playlists = {playlist};
    
    EXPECT_CALL(*mockPlaylistLibrary, getPlaylists())
        .WillRepeatedly(ReturnRef(playlists));
    EXPECT_CALL(*mockPlaylistView, getSelectedPlaylistID())
        .WillOnce(Return(1));
    EXPECT_CALL(*mockPlaylistLibrary, removePlaylist(playlistName))
        .Times(1);
    EXPECT_CALL(*mockPlaylistLibrary, saveToFile("playlists.txt"))
        .Times(1);
    
    playlistController->deletePlaylist();
}

TEST_F(PlaylistControllerTest, DeletePlaylist_InvalidID) {
    std::vector<Playlist> playlists = {Playlist("Rock Classics")}; // Giả lập có playlist
    EXPECT_CALL(*mockPlaylistLibrary, getPlaylists())
        .WillRepeatedly(ReturnRef(playlists));

    // Giả lập trường hợp người dùng chọn một ID không hợp lệ
    EXPECT_CALL(*mockPlaylistView, getSelectedPlaylistID())
        .WillOnce(Return(-1)); // ID không hợp lệ (Ví dụ: -1)

    // Đảm bảo rằng phương thức removePlaylist không được gọi
    EXPECT_CALL(*mockPlaylistLibrary, removePlaylist(_)).Times(0);
    EXPECT_CALL(*mockPlaylistLibrary, saveToFile(_)).Times(0);

    playlistController->deletePlaylist();
}



TEST_F(PlaylistControllerTest, ViewPlaylistDetails_Success) {
    std::string playlistName = "Rock Classics";
    Playlist playlist(playlistName);
    
    EXPECT_CALL(*mockPlaylistLibrary, getPlaylistByName(playlistName))
        .WillOnce(Return(&playlist));
    EXPECT_CALL(*mockPlaylistView, displayPlaylistDetails(testing::Ref(playlist)))
        .Times(1);
    
    playlistController->viewPlaylistDetails(playlistName);
}

TEST_F(PlaylistControllerTest, ViewPlaylistDetails_NotFound) {
    std::string playlistName = "Nonexistent Playlist";
    
    EXPECT_CALL(*mockPlaylistLibrary, getPlaylistByName(playlistName))
        .WillOnce(Return(nullptr));
    EXPECT_CALL(*mockPlaylistView, displayPlaylistDetails(_))
        .Times(0);
    
    playlistController->viewPlaylistDetails(playlistName);
}

TEST_F(PlaylistControllerTest, PlayPlaylist_Success) {
    std::string playlistName = "Rock Classics";
    Playlist playlist(playlistName);
    MediaFile song1(1, "Song1", "file1.mp3", "mp3");
    MediaFile song2(2, "Song2", "file2.mp3", "mp3");

    playlist.addSong(song1);
    playlist.addSong(song2);
    
    EXPECT_CALL(*mockPlaylistLibrary, getPlaylistByName(playlistName))
        .WillOnce(Return(&playlist));
    EXPECT_CALL(*mockManagerController, getManagerView())
        .WillRepeatedly(Return(mockManagerView.get()));
    EXPECT_CALL(*mockManagerView, setView("PlayingView"))
        .Times(1);
    EXPECT_CALL(*mockPlayingController, playPlaylist(_))
        .Times(1);
    
    playlistController->playPlaylist(playlistName);
}

TEST_F(PlaylistControllerTest, PlayPlaylist_NotFound) {
    std::string playlistName = "Nonexistent Playlist";
    
    EXPECT_CALL(*mockPlaylistLibrary, getPlaylistByName(playlistName))
        .WillOnce(Return(nullptr));
    EXPECT_CALL(*mockPlayingController, playPlaylist(::testing::_))
        .Times(0);
    
    playlistController->playPlaylist(playlistName);
}

TEST_F(PlaylistControllerTest, ListAllPlaylists_Success) {
    std::vector<Playlist> playlists = { Playlist("Rock Classics"), Playlist("Pop Hits") };
    EXPECT_CALL(*mockPlaylistLibrary, getPlaylists())
        .WillRepeatedly(ReturnRef(playlists));
    EXPECT_CALL(*mockPlaylistView, displayPlaylists(playlists))
        .Times(1);
    
    playlistController->listAllPlaylists();
}


TEST_F(PlaylistControllerTest, ListAllPlaylists_Empty) {
    std::vector<Playlist> playlists = {};  // ✅ Không có `const`
    
    EXPECT_CALL(*mockPlaylistLibrary, getPlaylists())
        .WillRepeatedly(ReturnRef(playlists));  // ✅ Phù hợp với `getPlaylists()`
    
    EXPECT_CALL(*mockPlaylistView, displayPlaylists(::testing::_))
        .Times(1);

    playlistController->listAllPlaylists();
}


TEST_F(PlaylistControllerTest, HandleAction_CreatePlaylist_Simplified) {
    std::string playlistName = "Test Playlist";

    EXPECT_CALL(*mockPlaylistLibrary, getPlaylistByName(playlistName))
        .WillOnce(Return(nullptr));
    EXPECT_CALL(*mockPlaylistLibrary, addPlaylist(_)).Times(1);
    EXPECT_CALL(*mockPlaylistLibrary, saveToFile("playlists.txt")).Times(1);

    // Gọi handleAction để kiểm tra toàn bộ flow
    playlistController->handleAction(ACTION_CREATE_PLAYLIST);
}


TEST_F(PlaylistControllerTest, HandleAction_DeletePlaylist_Simplified) {
    std::string playlistName = "Rock Classics";
    Playlist playlist(playlistName);
    std::vector<Playlist> playlists = {playlist};

    EXPECT_CALL(*mockPlaylistLibrary, getPlaylists())
        .WillRepeatedly(ReturnRef(playlists));

    EXPECT_CALL(*mockPlaylistView, getSelectedPlaylistID())
        .WillOnce(Return(1));  // Giả lập người dùng chọn playlist đầu tiên

    EXPECT_CALL(*mockPlaylistLibrary, removePlaylist(playlistName))
        .Times(1);

    EXPECT_CALL(*mockPlaylistLibrary, saveToFile("playlists.txt"))
        .Times(1);

    // Gọi handleAction thay vì deletePlaylist trực tiếp
    playlistController->handleAction(ACTION_DELETE_PLAYLIST);
}


TEST_F(PlaylistControllerTest, HandleAction_PlayPlaylists_Simplified) {
    std::string playlistName = "Rock Classics";
    Playlist playlist(playlistName);
    MediaFile song1(1, "Song1", "file1.mp3", "mp3");
    MediaFile song2(2, "Song2", "file2.mp3", "mp3");

    playlist.addSong(song1);
    playlist.addSong(song2);

    std::vector<Playlist> playlists = {playlist};

    EXPECT_CALL(*mockPlaylistLibrary, getPlaylists())
        .WillRepeatedly(ReturnRef(playlists));

    // Giả lập chọn playlist đầu tiên
    EXPECT_CALL(*mockPlaylistView, getSelectedPlaylistID())
        .WillOnce(Return(1));

    EXPECT_CALL(*mockPlaylistLibrary, getPlaylistByName(playlistName))
        .WillOnce(Return(&playlist));

    EXPECT_CALL(*mockManagerView, setView("PlayingView"))
        .Times(1);

    EXPECT_CALL(*mockPlayingController, playPlaylist(_))
        .Times(1);

    playlistController->handleAction(ACTION_PLAY_PLAYLISTS);
}

TEST_F(PlaylistControllerTest, HandleAction_ViewPlaylistDetails_Simplified) {
    std::string playlistName = "Rock Classics";
    Playlist playlist(playlistName);
    std::vector<Playlist> playlists = {playlist};

    EXPECT_CALL(*mockPlaylistLibrary, getPlaylists())
        .WillRepeatedly(ReturnRef(playlists));

    // Giả lập chọn playlist đầu tiên
    EXPECT_CALL(*mockPlaylistView, getSelectedPlaylistID())
        .WillOnce(Return(1));

    EXPECT_CALL(*mockPlaylistLibrary, getPlaylistByName(playlistName))
        .WillOnce(Return(&playlist));

    EXPECT_CALL(*mockPlaylistView, displayPlaylistDetails(testing::Ref(playlist)))
        .Times(1);

    // Gọi handleAction thay vì viewPlaylistDetails trực tiếp
    playlistController->handleAction(ACTION_VIEW_PLAYLIST_DETAILS);
}

TEST_F(PlaylistControllerTest, HandleAction_ExitPlaylistMenu_Simplified) {
    // Giả lập PlayingMediaController tồn tại và có thể được lấy từ ManagerController
    EXPECT_CALL(*mockManagerController, getController("PlayingView"))
        .WillRepeatedly(Return(mockPlayingController.get()));

    // Kiểm tra `stop()` được gọi để dừng media đang phát
    EXPECT_CALL(*mockPlayingController, stop())
        .Times(1);

    // Kiểm tra `setView("Default")` được gọi để quay về màn hình chính
    EXPECT_CALL(*mockManagerView, setView("Default"))
        .Times(1);

    // Gọi handleAction để kiểm tra toàn bộ quy trình
    playlistController->handleAction(ACTION_EXIT_PLAYLIST_MENU);
}

TEST_F(PlaylistControllerTest, HandleAction_ListAllPlaylists_Simplified) {
    std::vector<Playlist> playlists = {
        Playlist("Rock Classics"),
        Playlist("Pop Hits")
    };

    // Giả lập danh sách playlist
    EXPECT_CALL(*mockPlaylistLibrary, getPlaylists())
        .WillRepeatedly(ReturnRef(playlists));

    // Kiểm tra xem `displayPlaylists()` có được gọi với danh sách playlists hay không
    EXPECT_CALL(*mockPlaylistView, displayPlaylists(playlists))
        .Times(1);

    // Gọi handleAction để kiểm tra toàn bộ quy trình
    playlistController->handleAction(ACTION_LIST_ALL_PLAYLISTS);
}
TEST_F(PlaylistControllerTest, HandleActionCreatePlaylist_Cancelled) {
    std::vector<Playlist> emptyPlaylists;
    ON_CALL(*mockPlaylistLibrary, getPlaylists()).WillByDefault(ReturnRef(emptyPlaylists));
    ON_CALL(*mockPlaylistLibrary, getPlaylistByName(::testing::_)).WillByDefault(Return(nullptr));

    EXPECT_CALL(*mockPlaylistLibrary, addPlaylist(::testing::_)).Times(0);
    EXPECT_CALL(*mockPlaylistLibrary, saveToFile("playlists.txt")).Times(0);
    EXPECT_CALL(*mockPlaylistView, displayPlaylists(::testing::_)).Times(0);

    // ✅ Giả lập người dùng NHẤN ESC thay vì nhập dữ liệu
    testing::internal::CaptureStdout();
    playlistController->handleAction(ACTION_CREATE_PLAYLIST);
    std::string output = testing::internal::GetCapturedStdout();

    EXPECT_TRUE(output.find("Playlist creation cancelled.") != std::string::npos) 
        << "Expected output: 'Playlist creation cancelled.' but not found.";
}
TEST_F(PlaylistControllerTest, HandleActionCreatePlaylist_EmptyName) {
    std::vector<Playlist> emptyPlaylists;
    ON_CALL(*mockPlaylistLibrary, getPlaylists()).WillByDefault(ReturnRef(emptyPlaylists));
    ON_CALL(*mockPlaylistLibrary, getPlaylistByName(::testing::_)).WillByDefault(Return(nullptr));

    EXPECT_CALL(*mockPlaylistLibrary, addPlaylist(::testing::_)).Times(0);
    EXPECT_CALL(*mockPlaylistLibrary, saveToFile("playlists.txt")).Times(0);
    EXPECT_CALL(*mockPlaylistView, displayPlaylists(::testing::_)).Times(0);

    // ✅ Giả lập người dùng nhập tên RỖNG và nhấn ENTER
    testing::internal::CaptureStdout();
    playlistController->handleAction(ACTION_CREATE_PLAYLIST);
    std::string output = testing::internal::GetCapturedStdout();

    EXPECT_TRUE(output.find("Error: Playlist name cannot be empty.") != std::string::npos)
        << "Expected output: 'Error: Playlist name cannot be empty.' but not found.";
}

