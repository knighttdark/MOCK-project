// #include <gtest/gtest.h>
// #include <gmock/gmock.h>
// #include <vector>
// #include <string>
// #include <memory>
// #include "controller/PlayingMediaController.h"
// #include "model/MediaFile.h"
// #include "controller/ManagerController.h"
// #include "view/PlayingView.h"

// using namespace testing;
// using std::string;
// using std::vector;

// // ==========================
// // Mock Classes
// // ==========================

// // Mock cho PlayingMediaController
// class MockPlayingMediaController : public PlayingMediaController {
// public:
//     MOCK_METHOD(void, playMediaFile, (MediaFile* mediaFile), (override));
//     MOCK_METHOD(MediaFile*, getCurrentMediaFile, (), (const, override));
//     MOCK_METHOD(bool, isCurrentlyPlaying, (), (const, override));
//     MOCK_METHOD(void, stop, (), (override));
//     MOCK_METHOD(void, handleAction, (int action), (override));
//     MOCK_METHOD(void, skipToNext, (), (override));
//     MOCK_METHOD(void, skipToPrevious, (), (override));
//     MOCK_METHOD(void, adjustVolume, (int level), (override));
//     MOCK_METHOD(void, playVideo, (const string& videoPath), (override));
// };

// // Mock cho MediaFile
// class MockMediaFile : public MediaFile {
// public:
//     MOCK_METHOD(int, getIndex, (), (const, override));
//     MOCK_METHOD(std::string, getName, (), (const, override));
//     MOCK_METHOD(std::string, getPath, (), (const, override));
// };

// // Mock cho PlayingView
// class MockPlayingView : public PlayingView {
// public:
//     MOCK_METHOD(int, showMenu, (), (override));
// };

// // -------------------------
// // Test Fixture cho PlayingMediaController
// // -------------------------
// class PlayingMediaControllerTest : public ::testing::Test {
// protected:
//     std::unique_ptr<MockPlayingMediaController> mockPlayingMediaController;
//     std::unique_ptr<MockMediaFile> mockMediaFile;
//     std::unique_ptr<MockPlayingView> mockPlayingView;

//     void SetUp() override {
//         mockPlayingMediaController = std::make_unique<MockPlayingMediaController>();
//         mockMediaFile = std::make_unique<MockMediaFile>();
//         mockPlayingView = std::make_unique<MockPlayingView>();
//     }

//     void TearDown() override {
//         mockPlayingMediaController.reset();
//         mockMediaFile.reset();
//         mockPlayingView.reset();
//     }
// };

// // -------------------------
// // TEST CASES
// // -------------------------

// // ✅ Test stop()
// TEST_F(PlayingMediaControllerTest, Stop_WhenPlaying) {
//     EXPECT_CALL(*mockPlayingMediaController, stop())
//         .Times(1);

//     mockPlayingMediaController->stop();
// }

// // ✅ Test adjustVolume()
// TEST_F(PlayingMediaControllerTest, AdjustVolume_ValidLevel) {
//     EXPECT_CALL(*mockPlayingMediaController, adjustVolume(50)).Times(1);

//     mockPlayingMediaController->adjustVolume(50);
// }

// // ✅ Test playVideo()
// TEST_F(PlayingMediaControllerTest, PlayVideo_ValidFile) {
//     EXPECT_CALL(*mockPlayingMediaController, playVideo("/home/user/videos/test.mp4"))
//         .Times(1);

//     mockPlayingMediaController->playVideo("/home/user/videos/test.mp4");
// }
// // ✅ Test skipToPrevious()
// TEST_F(PlayingMediaControllerTest, SkipToPrevious) {
//     EXPECT_CALL(*mockPlayingMediaController, skipToPrevious()).Times(1);

//     mockPlayingMediaController->skipToPrevious();
// }

// // // ✅ Test handleAction()
// // TEST_F(PlayingMediaControllerTest, HandleAction_PlayPause) {
// //     EXPECT_CALL(*mockPlayingMediaController, handleAction(ACTION_PLAY_PAUSE))
// //         .Times(1);

// //     mockPlayingMediaController->handleAction(ACTION_PLAY_PAUSE);
// // }

// // TEST_F(PlayingMediaControllerTest, HandleAction_Stop) {
// //     EXPECT_CALL(*mockPlayingMediaController, stop()).Times(1);

// //     mockPlayingMediaController->handleAction(ACTION_STOP);
// // }

// // // ✅ Test skipToNext()
// // TEST_F(PlayingMediaControllerTest, SkipToNext) {
// //     EXPECT_CALL(*mockPlayingMediaController, skipToNext()).Times(1);

// //     mockPlayingMediaController->skipToNext();
// //}

// // ✅ Test playMediaFile()
// // TEST_F(PlayingMediaControllerTest, PlayMediaFile_ValidFile) {
// //     EXPECT_CALL(*mockMediaFile, getPath())
// //         .WillOnce(Return("/home/user/music/test.mp3"));

// //     EXPECT_CALL(*mockPlayingMediaController, playMediaFile(mockMediaFile.get()))
// //         .Times(1);

// //     mockPlayingMediaController->playMediaFile(mockMediaFile.get());
// // }
