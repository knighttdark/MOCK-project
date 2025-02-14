#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "controller/PlayingMediaController.h"
#include "controller/ManagerController.h"
#include "model/ManagerModel.h"
#include "view/PlayingView.h"  // ✅ Use PlayingView instead of PlayingMediaView
#include "common/MockClass.h"
#include <SDL2/SDL_mixer.h>

using namespace testing;

class PlayingMediaControllerTest : public ::testing::Test {
protected:
    std::unique_ptr<MockManagerController> mockManagerController;
    std::unique_ptr<MockManagerView> mockManagerView;
    std::unique_ptr<MockPlayingView> mockPlayingView;  // ✅ Replace PlayingMediaView with PlayingView
    std::unique_ptr<PlayingMediaController> playingMediaController;

    void SetUp() override {
        mockManagerController = std::make_unique<MockManagerController>();
        mockManagerView = std::make_unique<MockManagerView>();
        mockPlayingView = std::make_unique<MockPlayingView>();  // ✅ Use MockPlayingView

        // Set up ManagerController instance
        ManagerController::setInstance(mockManagerController.get());
        ManagerView::setInstance(mockManagerView.get());

        // Mock dependencies
        EXPECT_CALL(*mockManagerController, getManagerView())
            .WillRepeatedly(Return(mockManagerView.get()));

        EXPECT_CALL(*mockManagerView, getView())
            .WillRepeatedly(Return(mockPlayingView.get()));  // ✅ Use MockPlayingView

        // Inject mock view into PlayingMediaController
        playingMediaController = std::make_unique<PlayingMediaController>(mockManagerController.get(), mockPlayingView.get());
    }

    void TearDown() override {
        ManagerController::setInstance(nullptr);
        ManagerView::setInstance(nullptr);

        mockManagerController.reset();
        mockManagerView.reset();
        mockPlayingView.reset();
        playingMediaController.reset();
    }
};
TEST_F(PlayingMediaControllerTest, AdjustVolume_ValidRange) {
    EXPECT_NO_THROW(playingMediaController->adjustVolume(50));
    EXPECT_EQ(playingMediaController->getVolume(), 50);
}

TEST_F(PlayingMediaControllerTest, AdjustVolume_OutOfRange) {
    EXPECT_NO_THROW(playingMediaController->adjustVolume(-10));
    EXPECT_EQ(playingMediaController->getVolume(), 0);
    EXPECT_NO_THROW(playingMediaController->adjustVolume(150));
    EXPECT_EQ(playingMediaController->getVolume(), 100);
}

TEST_F(PlayingMediaControllerTest, PlayMediaFile_NullFile) {
    EXPECT_NO_THROW(playingMediaController->playMediaFile(nullptr));
}

TEST_F(PlayingMediaControllerTest, StopMedia) {
    playingMediaController->stop();
    EXPECT_FALSE(playingMediaController->isCurrentlyPlaying());
}

TEST_F(PlayingMediaControllerTest, HandleAction_PlayPause) {
    playingMediaController->handleAction(ACTION_PLAY_PAUSE);
    EXPECT_TRUE(playingMediaController->isCurrentlyPlaying());
    playingMediaController->handleAction(ACTION_PLAY_PAUSE);
    EXPECT_FALSE(playingMediaController->isCurrentlyPlaying());
}

TEST_F(PlayingMediaControllerTest, SkipToNextMedia) {
    playingMediaController->skipToNext();
    EXPECT_NE(playingMediaController->getCurrentMediaFile(), nullptr);
}

TEST_F(PlayingMediaControllerTest, SkipToPreviousMedia) {
    playingMediaController->skipToPrevious();
    EXPECT_NE(playingMediaController->getCurrentMediaFile(), nullptr);
}

TEST_F(PlayingMediaControllerTest, StartDisplayLoop) {
    EXPECT_NO_THROW(playingMediaController->startDisplayLoop());
}

TEST_F(PlayingMediaControllerTest, PlayPlaylist_Empty) {
    std::vector<MediaFile> playlist;
    playingMediaController->playPlaylist(playlist);
    EXPECT_EQ(playingMediaController->getCurrentMediaFile(), nullptr);
}

TEST_F(PlayingMediaControllerTest, RefreshPlayingView) {
    EXPECT_NO_THROW(playingMediaController->refreshPlayingView());
}

TEST_F(PlayingMediaControllerTest, PlayVideo_InvalidPath) {
    EXPECT_NO_THROW(playingMediaController->playVideo("invalid_path.mp4"));
}

TEST_F(PlayingMediaControllerTest, GetCurrentPlaylistIndex) {
    EXPECT_EQ(playingMediaController->getCurrentPlaylistIndex(), 0);
}

TEST_F(PlayingMediaControllerTest, GetCurrentPlaylist) {
    EXPECT_EQ(playingMediaController->getCurrentPlaylist(), nullptr);
}

// TEST_F(PlayingMediaControllerTest, SetCurrentMediaFile) {
//     MediaFile mediaFile("test.mp3");
//     playingMediaController->setCurrentMediaFile(&mediaFile);
//     EXPECT_EQ(playingMediaController->getCurrentMediaFile(), &mediaFile);
// }

TEST_F(PlayingMediaControllerTest, SetIsPlaying) {
    playingMediaController->setIsPlaying(true);
    EXPECT_TRUE(playingMediaController->isCurrentlyPlaying());
}

// TEST_F(PlayingMediaControllerTest, SetCurrentPlaylist) {
//     std::vector<MediaFile> playlist = {MediaFile("song1.mp3"), MediaFile("song2.mp3")};
//     playingMediaController->setCurrentPlaylist(&playlist);
//     EXPECT_EQ(playingMediaController->getCurrentPlaylist(), &playlist);
// }

TEST_F(PlayingMediaControllerTest, SetCurrentPlaylistIndex) {
    playingMediaController->setCurrentPlaylistIndex(1);
    EXPECT_EQ(playingMediaController->getCurrentPlaylistIndex(), 1);
}

TEST_F(PlayingMediaControllerTest, GetTotalTime) {
    EXPECT_EQ(playingMediaController->getTotalTime(), 0);
}

TEST_F(PlayingMediaControllerTest, GetCurrentTime) {
    EXPECT_EQ(playingMediaController->getCurrentTime(), 0);
}

