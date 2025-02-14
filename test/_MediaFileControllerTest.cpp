#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "common/MockClass.h"
#include <fstream>
using namespace testing;
using std::string;
using std::vector;
namespace fs = std::filesystem;

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "controller/MediaFileController.h"
#include "controller/ManagerController.h"
#include "model/ManagerModel.h"
#include "model/MediaLibrary.h"
#include "view/MediaFileView.h"
#include "common/MockClass.h"

using namespace testing;

class MediaFileControllerTest : public ::testing::Test {
protected:
    std::unique_ptr<MockManagerController> mockManagerController;
    std::unique_ptr<MockManagerView> mockManagerView;
    std::unique_ptr<MockManagerModel> mockManagerModel;
    std::unique_ptr<MockMediaLibrary> mockMediaLibrary;
    std::unique_ptr<MockPlaylistLibrary> mockPlaylistLibrary;  
    std::unique_ptr<MockPlaylistView> mockPlaylistView;   
    std::unique_ptr<MockMediaFileView> mockMediaFileView;
    std::unique_ptr<MediaFileController> mediaFileController;
    std::unique_ptr<MockPlayingMediaController> mockPlayingController;
    std::string username = std::getenv("USER");
    std::string mediaPath = "/home/" + username + "/test_media";

    void SetUp() override {
        mockManagerController = std::make_unique<MockManagerController>();
        mockManagerView = std::make_unique<MockManagerView>();
        mockManagerModel = std::make_unique<MockManagerModel>();
        mockMediaLibrary = std::make_unique<MockMediaLibrary>();
        mockPlaylistLibrary = std::make_unique<MockPlaylistLibrary>(); 
        mockPlaylistView = std::make_unique<MockPlaylistView>();
        mockPlayingController = std::make_unique<MockPlayingMediaController>();      
        mockMediaFileView = std::make_unique<MockMediaFileView>();

        ManagerController::setInstance(mockManagerController.get());
        ManagerView::setInstance(mockManagerView.get());
        ManagerModel::setInstance(mockManagerModel.get());
    


        EXPECT_CALL(*mockManagerController, getManagerView())
            .WillRepeatedly(Return(mockManagerView.get()));

        EXPECT_CALL(*mockManagerView, getView())
            .WillRepeatedly(Return(mockMediaFileView.get()));

        EXPECT_CALL(*mockManagerController, getManagerModel())
            .WillRepeatedly(Return(mockManagerModel.get()));

        EXPECT_CALL(*mockManagerModel, getMediaLibrary())
            .WillRepeatedly(ReturnRef(*mockMediaLibrary));
            
        EXPECT_CALL(*mockManagerController, getController("PlayingView"))
            .Times(testing::AnyNumber())  // Cho phép gọi nhiều lần
            .WillRepeatedly(Return(mockPlayingController.get()));

        std::vector<MediaFile> emptyMediaFiles;
        ON_CALL(*mockMediaLibrary, getMediaFiles()).WillByDefault(ReturnRef(emptyMediaFiles));

 
        ::testing::Mock::AllowLeak(mockMediaLibrary.get());

        mediaFileController = std::make_unique<MediaFileController>(mockManagerController.get());

        std::error_code ec;
        if (fs::exists(mediaPath)) {
            auto perms = fs::status(mediaPath).permissions();
            if ((perms & fs::perms::owner_write) != fs::perms::none) {
                fs::remove_all(mediaPath, ec);
            } else {
                std::cerr << "[WARNING] Cannot modify " << mediaPath << ". Skipping cleanup." << std::endl;
            }
        }


        
        fs::create_directory(mediaPath, ec);
        fs::create_directory(mediaPath + "/usb1", ec);
        fs::create_directory(mediaPath + "/usb2", ec);
    }

    void TearDown() override {
        std::error_code ec;
        if (fs::exists(mediaPath)) {
            auto perms = fs::status(mediaPath).permissions();
            if ((perms & fs::perms::owner_write) != fs::perms::none) {
                fs::remove_all(mediaPath, ec);  // Chỉ xóa nếu có quyền ghi
            } else {
                std::cerr << "[WARNING] Cannot modify " << mediaPath << ". Skipping cleanup." << std::endl;
            }
        }

        ManagerController::setInstance(nullptr);
        ManagerView::setInstance(nullptr);
        ManagerModel::setInstance(nullptr);

        mockManagerController.reset();
        mockManagerView.reset();
        mockManagerModel.reset();
        mockMediaLibrary.reset();
        mockMediaFileView.reset();
        mockPlayingController.reset();
    }
};


TEST_F(MediaFileControllerTest, HandleActionScan_USB_NoMediaDirectory) {
    fs::remove_all(mediaPath);  // Xóa thư mục

    EXPECT_NO_FATAL_FAILURE(mediaFileController->handleActionScan(SCAN_USB));
}


TEST_F(MediaFileControllerTest, HandleActionScan_USB_NoUSBDevices) {
    fs::remove_all(mediaPath);
    fs::create_directory(mediaPath);  // Thư mục trống

    EXPECT_NO_FATAL_FAILURE(mediaFileController->handleActionScan(SCAN_USB));
}


TEST_F(MediaFileControllerTest, HandleActionScan_USB_WithUSBDevices) {
    fs::create_directory(mediaPath + "/usb1");
    fs::create_directory(mediaPath + "/usb2");

    
    std::vector<std::string> detectedUSBs = {
        mediaPath + "/usb1",
        mediaPath + "/usb2",
        "/media/kist/Ubuntu 24.04.1 LTS amd64"  // Thêm trường hợp đặc biệt
    };

    
    // Giả lập việc quét USB đầu tiên mà không cần sự tương tác thực tế
    EXPECT_CALL(*mockMediaLibrary, scanDirectory(::testing::_)).Times(1);

    
    mediaFileController->handleActionScan(SCAN_USB);
}



TEST_F(MediaFileControllerTest, HandleActionScan_ReturnHome) {
    EXPECT_CALL(*mockManagerView, setView("Default")).Times(1);

    mediaFileController->handleActionScan(RETURN_HOME);
}


TEST_F(MediaFileControllerTest, HandleActionScan_InvalidOption) {
    EXPECT_NO_FATAL_FAILURE(mediaFileController->handleActionScan(-1));  // Giá trị không hợp lệ
}
TEST_F(MediaFileControllerTest, HandleActionScan_ScanDirectory) {
    std::cout << "DEBUG: Running HandleActionScan_ScanDirectory test." << std::endl;

    //  Ensure `promptDirectoryInput()` returns a valid path
    EXPECT_CALL(*mockMediaFileView, promptDirectoryInput())
        .WillOnce(Return("/home/user/Music"));

    //  Ensure `scanDirectory()` is executed
    EXPECT_CALL(*mockMediaLibrary, scanDirectory("/home/user/Music"))
        .Times(1);

    //  Call the function under test
    mediaFileController->handleActionScan(SCAN_DIRECTORY);

    std::cout << "DEBUG: Completed HandleActionScan_ScanDirectory test." << std::endl;
}

TEST_F(MediaFileControllerTest, HandleAction_EmptyMediaLibrary_ReturnsToDefaultView) {
    //  Ensure `getMediaFiles()` returns an empty list
    std::vector<MediaFile> emptyMediaFiles;  // Persistent empty vector
    EXPECT_CALL(*mockMediaLibrary, getMediaFiles())
        .WillOnce(ReturnRef(emptyMediaFiles));  //  Use `ReturnRef()`

    //  Expect `setView("Default")` to be called
    EXPECT_CALL(*mockManagerView, setView("Default"));

    //  Call function
    mediaFileController->handleAction(ACTION_SHOW_PROPERTIES);
}


TEST_F(MediaFileControllerTest, HandleAction_ShowProperties_ValidMedia) {
    std::vector<MediaFile> files = {MediaFile(1, "song.mp3", "/home/music/song.mp3", "audio/mp3")};

    EXPECT_CALL(*mockMediaLibrary, getMediaFiles())
        .WillRepeatedly(ReturnRef(files));

    EXPECT_CALL(*mockMediaFileView, getSelectedMediaID())
        .WillOnce(Return(1));

    //Allow two calls if expected
    EXPECT_CALL(*mockManagerView, setView("Metadata"))
        .Times(2);

    EXPECT_CALL(*mockManagerController, getController("Metadata"))
        .WillOnce(Return(new MetadataController()));

    mediaFileController->handleAction(ACTION_SHOW_PROPERTIES);
}


TEST_F(MediaFileControllerTest, HandleAction_ShowProperties_InvalidMedia) {
    std::vector<MediaFile> emptyMediaFiles;  // Create an empty media list

    EXPECT_CALL(*mockMediaLibrary, getMediaFiles())
        .WillRepeatedly(ReturnRef(emptyMediaFiles));

    // Allow `setView("Default")` to prevent early exit
    EXPECT_CALL(*mockManagerView, setView("Default"));

    // Ensure `getSelectedMediaID()` is actually called
    EXPECT_CALL(*mockMediaFileView, getSelectedMediaID())
        .Times(0);  // Since it's an invalid media case, it should NOT be called

    mediaFileController->handleAction(ACTION_SHOW_PROPERTIES);
}



TEST_F(MediaFileControllerTest, HandleAction_NextPage_WithMedia) {
    
    std::vector<MediaFile> mediaFiles = {
        MediaFile(1, "Song1.mp3", "/home/music/Song1.mp3", "audio/mp3"),
        MediaFile(2, "Song2.mp3", "/home/music/Song2.mp3", "audio/mp3")
    };

    EXPECT_CALL(*mockMediaLibrary, getMediaFiles())
        .WillRepeatedly(ReturnRef(mediaFiles));  

    
    EXPECT_CALL(*mockMediaLibrary, getTotalPages(_))
        .WillRepeatedly(Return(2));

    
    EXPECT_CALL(*mockMediaLibrary, getMediaFilesForPage(1, _))
        .WillOnce(Return(mediaFiles));

    
    EXPECT_CALL(*mockMediaFileView, displayMediaFiles(
        testing::ElementsAre("1. Song1.mp3", "2. Song2.mp3"),
        2, ""
    )).Times(1);

    
    mediaFileController->handleAction(ACTION_NEXT_PAGE);
}


TEST_F(MediaFileControllerTest, HandleAction_PlayMedia_ValidFile) {
    //  Fix: Provide all four required arguments
    std::vector<MediaFile> files = {MediaFile(1, "song.mp3", "/home/music/song.mp3", "audio/mp3")};

    //  Fix: Use `ReturnRef()` since `getMediaFiles()` returns a reference
    EXPECT_CALL(*mockMediaLibrary, getMediaFiles())
        .WillRepeatedly(ReturnRef(files));

    EXPECT_CALL(*mockMediaFileView, getSelectedMediaID())
        .WillOnce(Return(1));

    EXPECT_CALL(*mockManagerView, setView("PlayingView"));

    EXPECT_CALL(*mockManagerController, getController("PlayingView"))
        .WillOnce(Return(new PlayingMediaController()));

    mediaFileController->handleAction(ACTION_PLAY_MEDIA);
}


TEST_F(MediaFileControllerTest, HandleAction_PlayMedia_InvalidFile) {
    std::vector<MediaFile> emptyMediaFiles;  // Empty media list

    EXPECT_CALL(*mockMediaLibrary, getMediaFiles())
        .WillRepeatedly(ReturnRef(emptyMediaFiles));  // Return empty media list

    // Allow `setView("Default")` to prevent early exit
    EXPECT_CALL(*mockManagerView, setView("Default"));

    //  Since no media exists, `getSelectedMediaID()` should NOT be called
    EXPECT_CALL(*mockMediaFileView, getSelectedMediaID())
        .Times(0);

    mediaFileController->handleAction(ACTION_PLAY_MEDIA);
}

TEST_F(MediaFileControllerTest, HandleAction_AddToPlaylist_ValidFile) {
    //Fix: Provide all four required arguments
    std::vector<MediaFile> files = {MediaFile(1, "song.mp3", "/home/music/song.mp3", "audio/mp3")};

    //  Fix: Use a mocked PlaylistLibrary instead of std::vector<Playlist>
    MockPlaylistLibrary mockPlaylistLibrary;

    EXPECT_CALL(*mockMediaLibrary, getMediaFiles())
        .WillRepeatedly(ReturnRef(files));  // Use `ReturnRef()`

    EXPECT_CALL(*mockMediaFileView, getSelectedMediaID())
        .WillOnce(Return(1));

    EXPECT_CALL(*mockManagerModel, getPlaylistLibrary())
        .WillRepeatedly(ReturnRef(mockPlaylistLibrary));  // Use a proper mock

    EXPECT_CALL(*mockManagerView, setView("Playlist"));

    mediaFileController->handleAction(ACTION_ADD_TO_PLAYLIST);
}

TEST_F(MediaFileControllerTest, HandleAction_AddToPlaylist_NoPlaylists) {
    std::vector<MediaFile> files = {MediaFile(1, "song.mp3", "/home/music/song.mp3", "audio/mp3")};

    //  Fix: Use a mock PlaylistLibrary instead of std::vector<Playlist>
    MockPlaylistLibrary mockPlaylistLibrary;

    EXPECT_CALL(*mockMediaLibrary, getMediaFiles())
        .WillRepeatedly(ReturnRef(files));

    EXPECT_CALL(*mockMediaFileView, getSelectedMediaID())
        .WillOnce(Return(1));

    //  Fix: Return a mock `PlaylistLibrary` instead of an empty vector
    EXPECT_CALL(*mockManagerModel, getPlaylistLibrary())
        .WillRepeatedly(ReturnRef(mockPlaylistLibrary));

    mediaFileController->handleAction(ACTION_ADD_TO_PLAYLIST);
}

TEST_F(MediaFileControllerTest, HandleAction_ReturnToPlayingView) {
    // Fix: Provide a non-empty media library
    std::vector<MediaFile> mediaFiles = {
        MediaFile(1, "song.mp3", "/home/music/song.mp3", "audio/mp3")
    };

    EXPECT_CALL(*mockMediaLibrary, getMediaFiles())
        .WillRepeatedly(ReturnRef(mediaFiles));  //  Ensure media is available

    auto mockPlayingView = std::make_unique<MockPlayingView>();

    EXPECT_CALL(*mockManagerView, getView())
        .WillRepeatedly(Return(static_cast<BaseView*>(mockPlayingView.get())));

    EXPECT_CALL(*mockManagerView, setView("PlayingView"))
        .Times(1);

    mediaFileController->handleAction(ACTION_RETURN_TO_PLAYING);
}
TEST_F(MediaFileControllerTest, NextPage_HasMorePages) {
    //  Setup: Media library has multiple pages
    std::vector<MediaFile> filesPage1 = {
        MediaFile(1, "Song1.mp3", "/home/music/Song1.mp3", "audio/mp3"),
        MediaFile(2, "Song2.mp3", "/home/music/Song2.mp3", "audio/mp3")
    };

    std::vector<MediaFile> filesPage2 = {
        MediaFile(3, "Song3.mp3", "/home/music/Song3.mp3", "audio/mp3"),
        MediaFile(4, "Song4.mp3", "/home/music/Song4.mp3", "audio/mp3")
    };

    EXPECT_CALL(*mockManagerModel, getMediaLibrary())
        .WillRepeatedly(ReturnRef(*mockMediaLibrary));

    //  Expect media library to have 2 total pages
    EXPECT_CALL(*mockMediaLibrary, getTotalPages(_))
        .WillRepeatedly(Return(2));

    //  Expect page 1 media files to be returned first
    EXPECT_CALL(*mockMediaLibrary, getMediaFilesForPage(0, _))
        .WillRepeatedly(Return(filesPage1));

    //  Expect page 2 media files when moving to the next page
    EXPECT_CALL(*mockMediaLibrary, getMediaFilesForPage(1, _))
        .WillRepeatedly(Return(filesPage2));

    //  Expect `displayMediaFiles()` to be called with page 2 data
    EXPECT_CALL(*mockMediaFileView, displayMediaFiles(
        testing::ElementsAre("3. Song3.mp3", "4. Song4.mp3"),
        2, ""
    )).Times(1);

    //  Call function
    mediaFileController->nextPage();
}

TEST_F(MediaFileControllerTest, NextPage_LastPage) {
    //  Setup: Media library has only 1 page
    std::vector<MediaFile> filesPage1 = {
        MediaFile(1, "Song1.mp3", "/home/music/Song1.mp3", "audio/mp3")
    };

    EXPECT_CALL(*mockManagerModel, getMediaLibrary())
        .WillRepeatedly(ReturnRef(*mockMediaLibrary));

    //  Expect media library to have only 1 page
    EXPECT_CALL(*mockMediaLibrary, getTotalPages(_))
        .WillRepeatedly(Return(1));

    //  Ensure `getMediaFilesForPage()` returns actual media files
    EXPECT_CALL(*mockMediaLibrary, getMediaFilesForPage(0, _))
        .WillRepeatedly(Return(filesPage1));

    //  Expect `displayMediaFiles()` to be called with an error message
    EXPECT_CALL(*mockMediaFileView, displayMediaFiles(
        testing::ElementsAre("1. Song1.mp3"),
        1, "Already on the last page."
    )).Times(1);

    //  Call function when already at last page
    mediaFileController->nextPage();
}
TEST_F(MediaFileControllerTest, PreviousPage_NoPreviousPage) {
    // Setup: Media library has only 1 page
    std::vector<MediaFile> filesPage1 = {
        MediaFile(1, "Song1.mp3", "/home/music/Song1.mp3", "audio/mp3")
    };

    EXPECT_CALL(*mockManagerModel, getMediaLibrary())
        .WillRepeatedly(ReturnRef(*mockMediaLibrary));

    // Expect media library to have only 1 page
    EXPECT_CALL(*mockMediaLibrary, getMediaFilesForPage(0, _))
        .WillRepeatedly(Return(filesPage1));

    // Expect `displayMediaFiles()` to be called with an error message
    EXPECT_CALL(*mockMediaFileView, displayMediaFiles(
        testing::ElementsAre("1. Song1.mp3"),
        1, "Already on the first page."
    )).Times(1);

    // Call function when already at first page
    mediaFileController->previousPage();
}



TEST_F(MediaFileControllerTest, PreviousPage_FirstPage) {
    //  Setup: Media library has only 1 page
    std::vector<MediaFile> filesPage1 = {
        MediaFile(1, "Song1.mp3", "/home/music/Song1.mp3", "audio/mp3")
    };

    EXPECT_CALL(*mockManagerModel, getMediaLibrary())
        .WillRepeatedly(ReturnRef(*mockMediaLibrary));

    //  Expect media library to have only 1 page
    EXPECT_CALL(*mockMediaLibrary, getTotalPages(_))
        .WillRepeatedly(Return(1));

    //  Ensure `getMediaFilesForPage()` returns actual media files
    EXPECT_CALL(*mockMediaLibrary, getMediaFilesForPage(0, _))
        .WillRepeatedly(Return(filesPage1));

    //  Expect `displayMediaFiles()` to be called with an error message
    EXPECT_CALL(*mockMediaFileView, displayMediaFiles(
        testing::ElementsAre("1. Song1.mp3"),
        1, "Already on the first page."
    )).Times(1);

    //  Call function when already at first page
    mediaFileController->previousPage();
}

TEST_F(MediaFileControllerTest, ScanAndDisplayMedia_WithFiles) {
    
    std::vector<MediaFile> mediaFiles = {
        MediaFile(1, "Song1.mp3", "/home/music/Song1.mp3", "audio/mp3"),
        MediaFile(2, "Song2.mp3", "/home/music/Song2.mp3", "audio/mp3")
    };

    EXPECT_CALL(*mockManagerModel, getMediaLibrary())
        .WillRepeatedly(ReturnRef(*mockMediaLibrary));

    
    EXPECT_CALL(*mockMediaLibrary, getMediaFilesForPage(0, _))
        .WillRepeatedly(Return(mediaFiles));

    
    EXPECT_CALL(*mockMediaFileView, displayMediaFiles(
        testing::ElementsAre("1. Song1.mp3", "2. Song2.mp3"),
        1, ""  
    )).Times(1);

    
    mediaFileController->scanAndDisplayMedia();
}

TEST_F(MediaFileControllerTest, ScanAndDisplayMedia_NoFiles) {
    
    std::vector<MediaFile> emptyFiles;

    EXPECT_CALL(*mockManagerModel, getMediaLibrary())
        .WillRepeatedly(ReturnRef(*mockMediaLibrary));

    
    EXPECT_CALL(*mockMediaLibrary, getMediaFilesForPage(0, _))
        .WillRepeatedly(Return(emptyFiles));

    
    EXPECT_CALL(*mockMediaFileView, displayMediaFiles(
        testing::IsEmpty(),  
        1, ""  
    )).Times(1);

    
    mediaFileController->scanAndDisplayMedia();
}


TEST_F(MediaFileControllerTest, PreviousPage_MediaFileViewNull) {
    // ✅ Giả lập `getView()` trả về `nullptr` để kiểm tra nhánh `if (!mediaFileView)`
    EXPECT_CALL(*mockManagerView, getView()).WillOnce(Return(nullptr));

    // ✅ Gọi previousPage(), chương trình sẽ không crash mà in lỗi
    mediaFileController->previousPage();
}

TEST_F(MediaFileControllerTest, PreviousPage_WithPreviousPageAvailable) {
    // ✅ Giả lập danh sách media files
    std::vector<MediaFile> mediaFilesPage1 = {
        MediaFile(1, "Song1.mp3", "/home/music/Song1.mp3", "audio/mp3"),
        MediaFile(2, "Song2.mp3", "/home/music/Song2.mp3", "audio/mp3")
    };
    std::vector<MediaFile> mediaFilesPage2 = {
        MediaFile(3, "Song3.mp3", "/home/music/Song3.mp3", "audio/mp3"),
        MediaFile(4, "Song4.mp3", "/home/music/Song4.mp3", "audio/mp3")
    };

    // ✅ Mock số trang (tránh cảnh báo getTotalPages(25) trả về mặc định)
    EXPECT_CALL(*mockMediaLibrary, getTotalPages(_)).WillRepeatedly(Return(2));

    // ✅ Mock `getMediaLibrary()` và `getMediaFilesForPage()`
    EXPECT_CALL(*mockManagerModel, getMediaLibrary()).WillRepeatedly(ReturnRef(*mockMediaLibrary));
    EXPECT_CALL(*mockMediaLibrary, getMediaFilesForPage(0, _)).WillRepeatedly(Return(mediaFilesPage1));
    EXPECT_CALL(*mockMediaLibrary, getMediaFilesForPage(1, _)).WillRepeatedly(Return(mediaFilesPage2));

    // ✅ Mock `mediaFileView`
    EXPECT_CALL(*mockManagerView, getView()).WillRepeatedly(Return(mockMediaFileView.get()));

    // ✅ Mong đợi khi chuyển sang trang 2
    EXPECT_CALL(*mockMediaFileView, displayMediaFiles(testing::ElementsAre("3. Song3.mp3", "4. Song4.mp3"), 2, ""))
        .Times(1);

    // ✅ Mong đợi khi quay lại trang 1 (chỉnh lại thông báo đúng)
    EXPECT_CALL(*mockMediaFileView, displayMediaFiles(testing::ElementsAre("1. Song1.mp3", "2. Song2.mp3"), 1, ""))
        .Times(1);

    // ✅ Giả lập chuyển trang và quay lại
    mediaFileController->nextPage();    // Chuyển đến trang 2
    mediaFileController->previousPage(); // Quay lại trang 1
}



TEST_F(MediaFileControllerTest, GetPathById_NotFound) {
    // ✅ Giả lập danh sách media files
    std::vector<MediaFile> mediaFiles = {
        MediaFile(1, "Song1.mp3", "/home/music/Song1.mp3", "audio/mp3")
    };

    // ✅ Kiểm tra trường hợp `id` không khớp, phải trả về `""`
    std::string path = mediaFileController->getPathById(mediaFiles, 999);
    EXPECT_EQ(path, ""); // ✅ Giá trị trả về phải là chuỗi rỗng
}

TEST_F(MediaFileControllerTest, HandleAction_ReturnHome_Success) {
    // ✅ Đảm bảo `getController("PlayingView")` được gọi và trả về `mockPlayingController`
    EXPECT_CALL(*mockManagerController, getController("PlayingView"))
        .WillOnce(Return(mockPlayingController.get()));

    EXPECT_CALL(*mockPlayingController, stop()).Times(1); // ✅ Kiểm tra `stop()` được gọi
    EXPECT_CALL(*mockManagerView, setView("Default")).Times(1); // ✅ Kiểm tra giao diện chuyển về "Default"

    // ✅ Gọi action
    mediaFileController->handleAction(ACTION_RETURN_HOME);
}

TEST_F(MediaFileControllerTest, HandleAction_ReturnHome_NoPlayingController) {
    // ✅ Giả lập `getController("PlayingView")` trả về `nullptr`
    EXPECT_CALL(*mockManagerController, getController("PlayingView"))
        .WillOnce(Return(nullptr));

    // ✅ Kiểm tra `stop()` không được gọi
    EXPECT_CALL(*mockManagerView, setView("Default")).Times(0); // Không đổi giao diện

    // ✅ Gọi action
    mediaFileController->handleAction(ACTION_RETURN_HOME);
}