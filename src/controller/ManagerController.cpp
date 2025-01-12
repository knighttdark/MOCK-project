#include "controller/ManagerController.h"
#include <iostream>
#include "view/DefaultScreenView.h"
#include "controller/DefaultScreenController.h"
#include "view/MediaFileView.h"
#include "controller/MediaFileController.h"

// Private constructor
ManagerController::ManagerController() 
    : managerView(&ManagerView::getInstance()), 
      managerModel(&ManagerModel::getInstance()) {}


ManagerView* ManagerController::getManagerView(){ return managerView;}
ManagerModel* ManagerController::getManagerModel(){ return managerModel;}

BaseController* ManagerController::getController(const std::string& key) const {
    auto it = controllers.find(key);
    if (it != controllers.end()) {
        return it->second;
    }
    return nullptr; // Trả về nullptr nếu không tìm thấy
}

void ManagerController::setController(const std::string& key, BaseController* controller) {
    controllers[key] = controller; // Đăng ký hoặc cập nhật controller
}


ManagerController::~ManagerController() {
    for (auto& pair : controllers) {
        delete pair.second; // Giải phóng từng controller
    }
    controllers.clear();
}
// Static method to get the instance
ManagerController& ManagerController::getInstance() {
    static ManagerController instance;
    return instance;
}

void ManagerController::registerController(const std::string& key, BaseController* controller) {
    controllers[key] = controller;
}

void ManagerController::initializeViews() {
    ManagerView& managerView = ManagerView::getInstance();
    ManagerModel& managerModel = ManagerModel::getInstance();

    // Register DefaultScreenView and DefaultScreenController
    DefaultScreenView* defaultView = new DefaultScreenView();
    DefaultScreenController* defaultController = new DefaultScreenController();

    managerView.registerView("Default", defaultView);
    registerController("Default", defaultController);

    // Register MediaFileView and MediaFileController
    MediaFileView* mediaFileView = new MediaFileView();
    MediaFileController* mediaFileController = new MediaFileController();

    managerView.registerView("MediaFile", mediaFileView);
    registerController("MediaFile", mediaFileController);

    // Register MetadataView and MetadataController
    MetadataView* metaDataView = new MetadataView();
    MetadataController* metadataController = new MetadataController();

    managerView.registerView("Metadata", metaDataView);
    registerController("Metadata", metadataController);

    // Set the initial view
    managerView.setView("Default");
    
}

void ManagerController::run() {
    ManagerView& managerView = ManagerView::getInstance();
   
    while (true) {
        if (managerView.getView() == nullptr) {
            std::cerr << "Error: Current view is null!" << std::endl;
            break;
        }
        std::string currentViewKey = managerView.getCurrentViewKey();

        // Show menu and handle actions regardless of the current view
        managerView.getView()->showMenu();
        int action = managerView.getView()->handleInput();

        auto it = controllers.find(currentViewKey);
        if (it != controllers.end() && it->second != nullptr) {
            it->second->handleAction(action);
        } else {
            std::cerr << "Error: No controller found for view: " << currentViewKey << std::endl;
        }
    }
}