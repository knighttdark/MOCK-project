
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <vector>
#include <string>
#include <memory>
#include "controller/MediaFileController.h"
#include "controller/ManagerController.h"
#include "view/MediaFileView.h"
#include "view/ManagerView.h"
#include "controller/MetadataController.h"
#include "controller/PlayingMediaController.h"
#include "model/MediaLibrary.h"
#include "model/ManagerModel.h"
#include "model/MediaFile.h"
#include "view/PlayingView.h"

using namespace testing;
using std::string;
using std::vector;

// ==========================
// Mock Classes
// ==========================

// Mock cho ManagerController
class MockManagerController : public ManagerController {
public:
    MOCK_METHOD(BaseController*, getController, (const std::string& key), (const, override));
    MOCK_METHOD(ManagerView*, getManagerView, (), (override));
};

// Mock cho ManagerView
class MockManagerView : public ManagerView {
public:
    MOCK_METHOD(void, setView, (const std::string& key), (override));
    MOCK_METHOD(BaseView*, getView, (), (const, override));
    MOCK_METHOD(std::string, getCurrentViewKey, (), (const, override));
};

// Mock cho MediaFileView (dùng cho `scanAndDisplayMedia()`)
class MockMediaFileView : public MediaFileView {
public:
    MOCK_METHOD(int, getSelectedMediaID, (), (const, override));
    MOCK_METHOD(void, displayMediaFiles, (const std::vector<std::string>& files, int page, const std::string& notification), (override));
};

// Mock cho MetadataController (dùng để hiển thị metadata)
class MockMetadataController : public MetadataController {
public:
    MOCK_METHOD(void, handleShowMetadata, (const std::string& filepath), (override));
};

// Mock cho PlayingMediaController (dùng để phát nhạc)
class MockPlayingMediaController : public PlayingMediaController {
public:
    MOCK_METHOD(void, playMediaFile, (MediaFile* mediaFile), (override));
    MOCK_METHOD(void, stop, (), (override));
};

// Mock cho PlaylistView (hiển thị danh sách playlist)
class MockPlaylistView : public PlaylistView {
public:
    MOCK_METHOD(void, displayPlaylists, (const std::vector<Playlist>& playlists), (override));
    MOCK_METHOD(int, getSelectedPlaylistID, (), (const, override));
};

// Mock cho PlayingView (dùng khi quay lại màn hình phát nhạc)
class MockPlayingView : public PlayingView {
public:
    MOCK_METHOD(int, showMenu, (), (override));
};

// Mock cho MediaLibrary (quản lý danh sách media files)
class MockMediaLibrary : public MediaLibrary {
public:
    MOCK_METHOD(std::vector<MediaFile>&, getMediaFiles, (), (override));
    MOCK_METHOD(std::vector<MediaFile>, getMediaFilesForPage, (int page, int pageSize), (const, override));
    MOCK_METHOD(int, getTotalPages, (int pageSize), (const, override));
};

// Mock cho MediaFile (dùng để giả lập media file thực tế)
class MockMediaFile : public MediaFile {
public:
    MOCK_METHOD(int, getIndex, (), (const, override));
    MOCK_METHOD(std::string, getName, (), (const, override));
    MOCK_METHOD(std::string, getPath, (), (const, override));
};

// Mock cho Playlist (dùng để quản lý playlist)
class MockPlaylist : public Playlist {
public:
    MOCK_METHOD(std::string, getName, (), (const, override));
    MOCK_METHOD(void, addSong, (const MediaFile& file), (override));
};

// Mock cho PlaylistLibrary (quản lý danh sách playlist)
class MockPlaylistLibrary : public PlaylistLibrary {
public:
    MOCK_METHOD(std::vector<Playlist>&, getPlaylists, (), (override));
    MOCK_METHOD(void, saveToFile, (const std::string& filename), (const, override));
};

// Mock cho ManagerModel (lớp quản lý dữ liệu chính)
class MockManagerModel : public ManagerModel {
public:
    MOCK_METHOD(MediaLibrary&, getMediaLibrary, (), (override));
    MOCK_METHOD(PlaylistLibrary&, getPlaylistLibrary, (), (override));
    MOCK_METHOD(PlayingMediaModel&, getPlayingMedia, (), (override));
};


// -------------------------
// Test Fixture cho MediaFileController sử dụng MockManagerModel
// -------------------------
class MediaFileControllerTest : public ::testing::Test {
protected:
    std::unique_ptr<MockManagerController> mockManagerController;
    std::unique_ptr<MockManagerView> mockManagerView;
    std::unique_ptr<MockMediaFileView> mockMediaFileView;
    std::unique_ptr<MockMetadataController> mockMetadataController;
    std::unique_ptr<MockManagerModel> mockManagerModel;
    std::unique_ptr<MockMediaLibrary> mockMediaLibrary;
    std::unique_ptr<MockPlayingMediaController> mockPlayingMediaController;

    std::unique_ptr<MediaFileController> mediaFileController;
    vector<MediaFile> mediaFiles;

    void SetUp() override {
        // ✅ Khởi tạo mock objects
        mockManagerController = std::make_unique<MockManagerController>();
        mockManagerView = std::make_unique<MockManagerView>();
        mockMediaFileView = std::make_unique<MockMediaFileView>();
        mockMetadataController = std::make_unique<MockMetadataController>();
        mockManagerModel = std::make_unique<MockManagerModel>();
        mockMediaLibrary = std::make_unique<MockMediaLibrary>();
        mockPlayingMediaController = std::make_unique<MockPlayingMediaController>();

        // ✅ Cấu hình `ManagerModel`
        ON_CALL(*mockManagerModel, getMediaLibrary()).WillByDefault(ReturnRef(*mockMediaLibrary));

        // ✅ Thêm file `.mp3` thật vào danh sách mediaFiles
        mediaFiles.clear();
        mediaFiles.emplace_back(MediaFile(
            1, 
            "APT.mp3", 
            "/home/kist/Documents/workspace/MOCK-project/media/APT.mp3",
            "mp3"
        ));
        ON_CALL(*mockMediaLibrary, getMediaFiles()).WillByDefault(ReturnRef(mediaFiles));

        // ✅ Cấu hình `ManagerController`
        ON_CALL(*mockManagerController, getManagerView()).WillByDefault(Return(mockManagerView.get()));
        ON_CALL(*mockManagerController, getController("Metadata")).WillByDefault(Return(mockMetadataController.get()));
        ON_CALL(*mockManagerController, getController("PlayingView")).WillByDefault(Return(mockPlayingMediaController.get()));

        // ✅ Cấu hình `ManagerView`
        ON_CALL(*mockManagerView, getView()).WillByDefault(Return(mockMediaFileView.get()));

        // ✅ Giả lập ID hợp lệ
        ON_CALL(*mockMediaFileView, getSelectedMediaID()).WillByDefault(Return(1));

        // ✅ Thay thế instance của ManagerModel & ManagerController
        ManagerModel::setInstance(mockManagerModel.get());
        ManagerController::setInstance(mockManagerController.get());

        // ✅ Khởi tạo MediaFileController
        mediaFileController = std::make_unique<MediaFileController>();
    }

    void TearDown() override {
        // ✅ Reset singleton để không ảnh hưởng test khác
        ManagerModel::setInstance(nullptr);
        ManagerController::setInstance(nullptr);
    }
};

// // --- Test Case: ACTION_PLAY_MEDIA --- //
// TEST_F(MediaFileControllerTest, HandleActionPlayMedia) {
//     EXPECT_CALL(*mockManagerView, setView("PlayingView")).Times(1);
//     EXPECT_CALL(*mockPlayingMediaController, playMediaFile(_)).Times(1);
    
//     mediaFileController->handleAction(ACTION_PLAY_MEDIA);
// }

// // --- Test Case: ACTION_RETURN_HOME --- //
// TEST_F(MediaFileControllerTest, HandleActionReturnHome) {
//     EXPECT_CALL(*mockPlayingMediaController, stop()).Times(1);
//     EXPECT_CALL(*mockManagerView, setView("Default")).Times(1);
    
//     mediaFileController->handleAction(ACTION_RETURN_HOME);
// }

// // --- Test Case: ACTION_RETURN_TO_PLAYING --- //
// TEST_F(MediaFileControllerTest, HandleActionReturnToPlaying) {
//     MockPlayingView mockPlayingView;
//     ON_CALL(*mockManagerView, getView()).WillByDefault(Return(&mockPlayingView));

//     EXPECT_CALL(*mockManagerView, setView("PlayingView")).Times(1);
//     EXPECT_CALL(mockPlayingView, showMenu()).Times(1);
    
//     mediaFileController->handleAction(ACTION_RETURN_TO_PLAYING);
// }



// TEST_F(MediaFileControllerTest, ScanAndDisplayMedia) {
//     EXPECT_CALL(*mockManagerView, getView())
//         .WillOnce(Return(mockMediaFileView.get()));

//     vector<MediaFile> mockFiles = {
//         MediaFile(1, "song1.mp3", "/media/song1.mp3", "mp3"),
//         MediaFile(2, "song2.mp3", "/media/song2.mp3", "mp3")
//     };

//     EXPECT_CALL(*mockMediaLibrary, getMediaFilesForPage(0, _))
//         .WillOnce(Return(mockFiles));

//     vector<string> expectedDisplay = {
//         "1. song1.mp3",
//         "2. song2.mp3"
//     };

//     EXPECT_CALL(*mockMediaFileView, displayMediaFiles(expectedDisplay, 1, ""));

//     mediaFileController->scanAndDisplayMedia();
// }
// TEST_F(MediaFileControllerTest, NextPage_ValidPageChange) {
//     EXPECT_CALL(*mockManagerView, getView())
//         .WillOnce(Return(mockMediaFileView.get()));

//     EXPECT_CALL(*mockMediaLibrary, getTotalPages(_))
//         .WillOnce(Return(3));

//     EXPECT_CALL(*mockMediaLibrary, getMediaFilesForPage(1, _))
//         .WillOnce(Return(vector<MediaFile>{
//             MediaFile(3, "song3.mp3", "/media/song3.mp3", "mp3"),
//             MediaFile(4, "song4.mp3", "/media/song4.mp3", "mp3")
//         }));

//     vector<string> expectedDisplay = {
//         "3. song3.mp3",
//         "4. song4.mp3"
//     };

//     EXPECT_CALL(*mockMediaFileView, displayMediaFiles(expectedDisplay, 2, ""));

//     mediaFileController->nextPage();
// }
// TEST_F(MediaFileControllerTest, PreviousPage_ValidPageChange) {
//     EXPECT_CALL(*mockManagerView, getView())
//         .WillOnce(Return(mockMediaFileView.get()));

//     EXPECT_CALL(*mockMediaLibrary, getMediaFilesForPage(0, _))
//         .WillOnce(Return(vector<MediaFile>{
//             MediaFile(1, "song1.mp3", "/media/song1.mp3", "mp3"),
//             MediaFile(2, "song2.mp3", "/media/song2.mp3", "mp3")
//         }));

//     vector<string> expectedDisplay = {
//         "1. song1.mp3",
//         "2. song2.mp3"
//     };

//     EXPECT_CALL(*mockMediaFileView, displayMediaFiles(expectedDisplay, 1, ""));

//     mediaFileController->previousPage();
// }
TEST_F(MediaFileControllerTest, GetPathById_ValidId) {
    vector<MediaFile> mediaFiles = {
        MediaFile(1, "song1.mp3", "/media/song1.mp3", "mp3"),
        MediaFile(2, "song2.mp3", "/media/song2.mp3", "mp3")
    };

    EXPECT_EQ(mediaFileController->getPathById(mediaFiles, 1), "/media/song1.mp3");
}

TEST_F(MediaFileControllerTest, GetPathById_InvalidId) {
    vector<MediaFile> mediaFiles = {
        MediaFile(1, "song1.mp3", "/media/song1.mp3", "mp3"),
        MediaFile(2, "song2.mp3", "/media/song2.mp3", "mp3")
    };

    EXPECT_EQ(mediaFileController->getPathById(mediaFiles, 99), "");
}
