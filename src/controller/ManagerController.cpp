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

ManagerController* ManagerController::instance = nullptr;

/* Constructor for ManagerController */
ManagerController::ManagerController()
    : managerView(&ManagerView::getInstance()),
      managerModel(&ManagerModel::getInstance()) {

        /* Load media files and playlists from file */
        managerModel->getPlaylistLibrary().loadFromFile("playlists.txt");
        cout << "Playlists loaded successfully from file.\n";
}

/* Get the ManagerView instance */
ManagerView* ManagerController::getManagerView() {
    return managerView;
}

/* Get the ManagerModel instance */
ManagerModel* ManagerController::getManagerModel() {
    return managerModel;
}


/* Get a controller by key */
BaseController* ManagerController::getController(const string& key) const {
    auto it = controllers.find(key);
    if (it != controllers.end()) {
        return it->second;
    }
    return nullptr;
}

// /* Set a controller by key */
// void ManagerController::setController(const string& key, BaseController* controller) {
//     controllers[key] = controller;
// }

/* Destructor for ManagerController */
ManagerController::~ManagerController() {
    for (auto& pair : controllers) {
        delete pair.second;
    }
    controllers.clear();
}

/* Get the singleton instance of ManagerController */
ManagerController& ManagerController::getInstance() {
    static ManagerController instance;
    return instance;
}

/* Register a controller with a key */
void ManagerController::registerController(const string& key, BaseController* controller) {
    controllers[key] = controller;
}

/* Initialize views for the application */
void ManagerController::initializeViews() {
    /* Register views and controllers */
    ManagerView& managerView = ManagerView::getInstance();

    /* Register default view and controller */
    BaseView* defaultView = new DefaultScreenView();
    BaseController* defaultController = new DefaultScreenController();
    managerView.registerView("Default", defaultView);
    registerController("Default", defaultController);

    /* Register MediaFile view and controller */
    BaseView* mediaFileView = new MediaFileView();
    BaseController* mediaFileController = new MediaFileController();
    managerView.registerView("MediaFile", mediaFileView);
    registerController("MediaFile", mediaFileController);

    /* Register Playlist view and controller */
    BaseView* metaDataView = new MetadataView();
    BaseController* metadataController = new MetadataController();
    managerView.registerView("Metadata", metaDataView);
    registerController("Metadata", metadataController);

    /* Register Playlist view and controller */
    BaseView* playlistView = new PlaylistView();
    BaseController* playlistController = new PlaylistController();
    managerView.registerView("Playlist", playlistView);
    registerController("Playlist", playlistController);

    /* Register Playing view and controller */
    PlayingView* playingView = new PlayingView();
    BaseController* playingMediaController = new PlayingMediaController();
    managerView.registerView("PlayingView", playingView);
    registerController("PlayingView", playingMediaController);

    /* Set the default view */
    managerView.setView("Default");
}

/* Run the application */
void ManagerController::run(bool isTest) {
    /* Get the singleton instance of ManagerView */
    ManagerView& managerView = ManagerView::getInstance();

    int loopCounter = 0;

    while (true) {
        /* Get the key of the current view */
        string currentViewKey = managerView.getCurrentViewKey();

        /* Show the menu for the current view */
        int action = managerView.getView()->showMenu();


        /* Get the controller for the current view */
        auto it = controllers.find(currentViewKey);
        if (it != controllers.end() && it->second != nullptr) {
            it->second->handleAction(action);
        } else {
            cerr << "Error: No controller found for view: " << currentViewKey << endl;
        }
    }
}
