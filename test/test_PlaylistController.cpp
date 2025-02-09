

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
