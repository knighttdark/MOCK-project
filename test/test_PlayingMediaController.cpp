// #include <gtest/gtest.h>
// #include <gmock/gmock.h>
// #include "controller/PlayingMediaController.h"
// #include "controller/ManagerController.h"
// #include "model/ManagerModel.h"
// #include "view/PlayingView.h"  // ✅ Use PlayingView instead of PlayingMediaView
// #include "common/MockClass.h"
// #include <SDL2/SDL_mixer.h>

// using namespace testing;

// class PlayingMediaControllerTest : public ::testing::Test {
// protected:
//     std::unique_ptr<MockManagerController> mockManagerController;
//     std::unique_ptr<MockManagerView> mockManagerView;
//     std::unique_ptr<MockPlayingView> mockPlayingView;  // ✅ Replace PlayingMediaView with PlayingView
//     std::unique_ptr<PlayingMediaController> playingMediaController;

//     void SetUp() override {
//         mockManagerController = std::make_unique<MockManagerController>();
//         mockManagerView = std::make_unique<MockManagerView>();
//         mockPlayingView = std::make_unique<MockPlayingView>();  // ✅ Use MockPlayingView

//         // Set up ManagerController instance
//         ManagerController::setInstance(mockManagerController.get());
//         ManagerView::setInstance(mockManagerView.get());

//         // Mock dependencies
//         EXPECT_CALL(*mockManagerController, getManagerView())
//             .WillRepeatedly(Return(mockManagerView.get()));

//         EXPECT_CALL(*mockManagerView, getView())
//             .WillRepeatedly(Return(mockPlayingView.get()));  // ✅ Use MockPlayingView

//         // Inject mock view into PlayingMediaController
//         playingMediaController = std::make_unique<PlayingMediaController>(mockManagerController.get(), mockPlayingView.get());
//     }

//     void TearDown() override {
//         ManagerController::setInstance(nullptr);
//         ManagerView::setInstance(nullptr);

//         mockManagerController.reset();
//         mockManagerView.reset();
//         mockPlayingView.reset();
//         playingMediaController.reset();
//     }
// };
// TEST_F(PlayingMediaControllerTest, AdjustVolume_ValidValues) {
//     playingMediaController->adjustVolume(50);
//     ASSERT_EQ(playingMediaController->getVolume(), 50);

//     playingMediaController->adjustVolume(100);
//     ASSERT_EQ(playingMediaController->getVolume(), 100);

//     playingMediaController->adjustVolume(0);
//     ASSERT_EQ(playingMediaController->getVolume(), 0);
// }
// TEST_F(PlayingMediaControllerTest, AdjustVolume_InvalidValues) {
//     playingMediaController->adjustVolume(-10);
//     ASSERT_NE(playingMediaController->getVolume(), -10);

//     playingMediaController->adjustVolume(120);
//     ASSERT_NE(playingMediaController->getVolume(), 120);
// }
// TEST_F(PlayingMediaControllerTest, Stop_NoMusicPlaying) {
//     EXPECT_CALL(*mockPlayingView, clearView()).Times(0);
//     EXPECT_CALL(*mockPlayingView, displayPlayingView(_, _, _, _)).Times(0);

//     playingMediaController->stop();
    
//     ASSERT_FALSE(playingMediaController->isCurrentlyPlaying());
// }
// TEST_F(PlayingMediaControllerTest, Stop_MusicPlaying) {
//     playingMediaController->setIsPlaying(true);

//     EXPECT_CALL(*mockPlayingView, clearView()).Times(1);
//     EXPECT_CALL(*mockPlayingView, displayPlayingView(_, _, _, _)).Times(1);

//     playingMediaController->stop();

//     ASSERT_FALSE(playingMediaController->isCurrentlyPlaying());
// }
// TEST(PlayingMediaControllerStandaloneTest, InitializeSDL) {
//     initializeSDL();
//     ASSERT_TRUE(isSDLInitialized);
// }
// TEST(PlayingMediaControllerStandaloneTest, CleanupSDL) {
//     initializeSDL();
//     cleanupSDL();

//     ASSERT_EQ(currentMusic, nullptr);
//     ASSERT_FALSE(isSDLInitialized);
// }
