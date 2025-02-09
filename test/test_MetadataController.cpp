#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "controller/MetadataController.h"
#include "controller/ManagerController.h"
#include "model/ManagerModel.h"
#include "view/MetadataView.h"
#include "model/MediaFile.h"
#include "common/MockClass.h"
#include "controller/MediaFileController.h"

using namespace testing;

class MetadataControllerTest : public ::testing::Test {
protected:
    std::unique_ptr<MockManagerController> mockManagerController;
    std::unique_ptr<MockManagerView> mockManagerView;
    std::unique_ptr<MockManagerModel> mockManagerModel;
    std::unique_ptr<MockMediaFileController> mockMediaFileController;
    std::unique_ptr<MockMetadataView> mockMetadataView;
    std::unique_ptr<MetadataController> metadataController;

    void SetUp() override {
        mockManagerController = std::make_unique<MockManagerController>();
        mockManagerView = std::make_unique<MockManagerView>();
        mockManagerModel = std::make_unique<MockManagerModel>();
        mockMediaFileController = std::make_unique<MockMediaFileController>();
        mockMetadataView = std::make_unique<MockMetadataView>();

        // Set up ManagerController instance
        ManagerController::setInstance(mockManagerController.get());
        ManagerView::setInstance(mockManagerView.get());
        ManagerModel::setInstance(mockManagerModel.get());

        // Mock dependencies
        EXPECT_CALL(*mockManagerController, getManagerView())
            .WillRepeatedly(Return(mockManagerView.get()));

        EXPECT_CALL(*mockManagerController, getManagerModel())
            .WillRepeatedly(Return(mockManagerModel.get()));

        EXPECT_CALL(*mockManagerController, getController("MediaFile"))
            .WillRepeatedly(Return(mockMediaFileController.get()));

        // Create MetadataController with mocked ManagerController
        //metadataController = std::make_unique<MetadataController>(mockManagerController.get());
        metadataController = std::make_unique<MetadataController>(mockManagerController.get(), mockMetadataView.get());

    }

    void TearDown() override {
        ManagerController::setInstance(nullptr);
        ManagerView::setInstance(nullptr);
        ManagerModel::setInstance(nullptr);

        mockManagerController.reset();
        mockManagerView.reset();
        mockManagerModel.reset();
        mockMediaFileController.reset();
        mockMetadataView.reset();
        metadataController.reset();
    }
};
TEST_F(MetadataControllerTest, HandleShowMetadata_FileExists) {
    std::string filepath = "/home/kist/Documents/workspace/MOCK-project/media/APT.mp3";

    // Tạo FileRef từ file thật
    TagLib::FileRef fileRef(filepath.c_str());
    
    // Đảm bảo file hợp lệ
    ASSERT_FALSE(fileRef.isNull());
    ASSERT_NE(fileRef.tag(), nullptr);
    ASSERT_NE(fileRef.audioProperties(), nullptr);

    ASSERT_NE(metadataController, nullptr);
    ASSERT_NE(mockMetadataView, nullptr);

    // Kiểm tra xem metadata được hiển thị đúng
    EXPECT_CALL(*mockMetadataView, displayMetadata(_)).Times(1);

    metadataController->handleShowMetadata(filepath);
}
