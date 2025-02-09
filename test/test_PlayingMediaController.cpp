
// #include "common/MockClass.h"
// using namespace testing;
// using std::string;
// using std::vector;

// class PlayingMediaControllerTest : public ::testing::Test {
// protected:
//     std::unique_ptr<MockManagerController> mockManagerController;
//     std::unique_ptr<MockManagerView> mockManagerView;
//     std::unique_ptr<MockManagerModel> mockManagerModel;
//     std::unique_ptr<MockMediaLibrary> mockMediaLibrary;
//     std::unique_ptr<MockPlayingView> mockPlayingView;
//     std::unique_ptr<PlayingMediaController> playingMediaController;

//     void SetUp() override {
//         mockManagerController = std::make_unique<MockManagerController>();
//         mockManagerView = std::make_unique<MockManagerView>();
//         mockManagerModel = std::make_unique<MockManagerModel>();
//         mockMediaLibrary = std::make_unique<MockMediaLibrary>();
//         mockPlayingView = std::make_unique<MockPlayingView>();

//         // ✅ Ensure `getManagerView()` returns `mockManagerView`
//         EXPECT_CALL(*mockManagerController, getManagerView())
//             .WillRepeatedly(Return(mockManagerView.get()));

//         // ✅ Ensure `getView()` returns `mockPlayingView`
//         EXPECT_CALL(*mockManagerView, getView())
//             .WillRepeatedly(Return(mockPlayingView.get()));

//         // ✅ Ensure `getManagerModel()` returns `mockManagerModel`
//         EXPECT_CALL(*mockManagerController, getManagerModel())
//             .WillRepeatedly(Return(mockManagerModel.get()));

//         // ✅ Ensure `getMediaLibrary()` returns `mockMediaLibrary`
//         EXPECT_CALL(*mockManagerModel, getMediaLibrary())
//             .WillRepeatedly(ReturnRef(*mockMediaLibrary));

//         // ✅ Prevent Google Mock from deleting objects prematurely
//         ::testing::Mock::AllowLeak(mockMediaLibrary.get());

//         // ✅ Create an instance of `PlayingMediaController` with the mocked manager controller
//         playingMediaController = std::make_unique<PlayingMediaController>(mockManagerController.get());
//     }

//     void TearDown() override {
//         ManagerController::setInstance(nullptr);
//         ManagerView::setInstance(nullptr);
//         ManagerModel::setInstance(nullptr);

//         mockManagerController.reset();
//         mockManagerView.reset();
//         mockManagerModel.reset();
//         mockMediaLibrary.reset();
//         mockPlayingView.reset();
//     }
// };

// // ==========================
// // Test Fixture
// // ==========================

// class PlayingMediaControllerTest : public ::testing::Test {
// protected:
//     std::unique_ptr<MockPlayingMediaController> mockPlayingMediaController;
//     std::unique_ptr<MockMediaFile> mockMediaFile;
//     std::unique_ptr<MockPlayingView> mockPlayingView;

//     void SetUp() override {
//         mockPlayingMediaController = std::make_unique<MockPlayingMediaController>();
//         mockMediaFile = std::make_unique<MockMediaFile>();
//         mockPlayingView = std::make_unique<MockPlayingView>();

//         // ✅ Đảm bảo stop() không gây lỗi bộ nhớ
//         ON_CALL(*mockPlayingMediaController, stop()).WillByDefault(Invoke([]() {
//             std::cout << "Mocked stop() called\n";
//         }));

//         // Thiết lập kỳ vọng cho mock
//         EXPECT_CALL(*mockPlayingMediaController, stop()).Times(AtMost(1));
//     }

//     void TearDown() override {
//         // Đảm bảo set lại instance thành nullptr để tránh core dump
//         if (ManagerController::getInstance() != nullptr) {
//             ManagerController::SetMockInstance(nullptr);  // Reset instance nếu không null
//         }

//         testing::Mock::VerifyAndClearExpectations(mockPlayingMediaController.get());
//         testing::Mock::VerifyAndClearExpectations(mockMediaFile.get());
//         testing::Mock::VerifyAndClearExpectations(mockPlayingView.get());

//         // Dọn dẹp bộ nhớ rõ ràng
//         mockPlayingMediaController.reset();
//         mockMediaFile.reset();
//         mockPlayingView.reset();
//     }
// };

// // ==========================
// // TEST CASES
// // ==========================

// // ✅ Test handleAction for Stop
// TEST_F(PlayingMediaControllerTest, HandleAction_Stop) {
//     EXPECT_CALL(*mockPlayingMediaController, stop()).Times(1);
//     mockPlayingMediaController->handleAction(ACTION_STOP);
// }

// // ✅ Test skipToNext (without calling stop directly)
// TEST_F(PlayingMediaControllerTest, SkipToNext) {
//     testing::InSequence seq;
//     EXPECT_CALL(*mockPlayingMediaController, skipToNext()).Times(1);
//     mockPlayingMediaController->skipToNext();
// }

// // ✅ Test adjustVolume
// TEST_F(PlayingMediaControllerTest, AdjustVolume_ValidLevel) {
//     EXPECT_CALL(*mockPlayingMediaController, adjustVolume(50)).Times(1);
//     mockPlayingMediaController->adjustVolume(50);
// }

// // ✅ Test playMediaFile
// TEST_F(PlayingMediaControllerTest, PlayMediaFile_ValidFile) {
//     EXPECT_CALL(*mockMediaFile, getPath()).WillOnce(Return("/home/user/music/test.mp3"));
//     EXPECT_CALL(*mockPlayingMediaController, playMediaFile(mockMediaFile.get())).Times(1);
//     mockPlayingMediaController->playMediaFile(mockMediaFile.get());
// }

// // ✅ Test skipToPrevious (without stop() being invoked directly)
// TEST_F(PlayingMediaControllerTest, SkipToPrevious) {
//     testing::InSequence seq;
//     EXPECT_CALL(*mockPlayingMediaController, skipToPrevious()).Times(1);
//     mockPlayingMediaController->skipToPrevious();
// }

// // ✅ Test handleAction for PlayPause
// TEST_F(PlayingMediaControllerTest, HandleAction_PlayPause) {
//     EXPECT_CALL(*mockPlayingMediaController, handleAction(ACTION_PLAY_PAUSE)).Times(1);
//     mockPlayingMediaController->handleAction(ACTION_PLAY_PAUSE);
// }

// // ✅ Test if stop() works correctly after skipping to the next track
// TEST_F(PlayingMediaControllerTest, SkipToNextAndStop) {
//     testing::InSequence seq;
//     EXPECT_CALL(*mockPlayingMediaController, stop()).Times(1);
//     EXPECT_CALL(*mockPlayingMediaController, skipToNext()).Times(1);

//     mockPlayingMediaController->skipToNext();  // Ensure stop() is called before skipToNext
// }

// // ✅ Test if stop() works correctly after skipping to the previous track
// TEST_F(PlayingMediaControllerTest, SkipToPreviousAndStop) {
//     testing::InSequence seq;
//     EXPECT_CALL(*mockPlayingMediaController, stop()).Times(1);
//     EXPECT_CALL(*mockPlayingMediaController, skipToPrevious()).Times(1);

//     mockPlayingMediaController->skipToPrevious();  // Ensure stop() is called before skipToPrevious
// }