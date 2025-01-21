#include "controller/ManagerController.h"
#include <iostream>
#include "view/DefaultScreenView.h"
#include "controller/DefaultScreenController.h"
#include "view/MediaFileView.h"
#include "controller/MediaFileController.h"
#include "controller/PlaylistController.h"
#include "controller/PlayingMediaController.h"
#include "view/PlayingView.h"
#include <stdexcept>


ManagerController::ManagerController()
    : managerView(&ManagerView::getInstance()),
      managerModel(&ManagerModel::getInstance()) {
    try {
        
        managerModel->getPlaylistLibrary().loadFromFile("playlists.txt");
        cout << "Playlists loaded successfully from file.\n";
    } catch (const exception& e) {
        
        cerr << "Error loading playlists: " << e.what() << '\n';
    }
}


ManagerView* ManagerController::getManagerView() {
    return managerView;
}


ManagerModel* ManagerController::getManagerModel() {
    return managerModel;
}


BaseController* ManagerController::getController(const string& key) const {
    auto it = controllers.find(key);
    if (it != controllers.end()) {
        return it->second;
    }
    
    return nullptr;
}


void ManagerController::setController(const string& key, BaseController* controller) {
    controllers[key] = controller;
}


ManagerController::~ManagerController() {
    for (auto& pair : controllers) {
        delete pair.second;
    }
    controllers.clear();
}


ManagerController& ManagerController::getInstance() {
    static ManagerController instance;
    return instance;
}


void ManagerController::registerController(const string& key, BaseController* controller) {
    controllers[key] = controller;
}


void ManagerController::initializeViews() {
    ManagerView& managerView = ManagerView::getInstance();
    ManagerModel& managerModel = ManagerModel::getInstance();

    

    BaseView* defaultView = new DefaultScreenView();
    BaseController* defaultController = new DefaultScreenController();

    managerView.registerView("Default", defaultView);
    registerController("Default", defaultController);

    
    BaseView* mediaFileView = new MediaFileView();
    BaseController* mediaFileController = new MediaFileController();

    managerView.registerView("MediaFile", mediaFileView);
    registerController("MediaFile", mediaFileController);

    
    BaseView* metaDataView = new MetadataView();
    BaseController* metadataController = new MetadataController();

    managerView.registerView("Metadata", metaDataView);
    registerController("Metadata", metadataController);

    
    BaseView* playlistView = new PlaylistView();
    BaseController* playlistController = new PlaylistController();

    managerView.registerView("Playlist", playlistView);
    registerController("Playlist", playlistController);

    PlayingView* playingView = new PlayingView();
    BaseController* playingMediaController = new PlayingMediaController(); 

    managerView.registerView("PlayingView", playingView);
    registerController("PlayingView", playingMediaController);

    
    managerView.setView("Default");
}


void ManagerController::run() {
    ManagerView& managerView = ManagerView::getInstance();

    while (true) {
        
        if (managerView.getView() == nullptr) {
            cerr << "Error: Current view is null!" << endl;
            break;
        }

        
        string currentViewKey = managerView.getCurrentViewKey();

        int action = managerView.getView()->showMenu();
        auto it = controllers.find(currentViewKey);
        if (it != controllers.end() && it->second != nullptr) {
            it->second->handleAction(action);
        } else {
            cerr << "Error: No controller found for view: " << currentViewKey << endl;
        }
    }
}
