#include "controller/ManagerController.h"
#include <iostream>
#include "view/DefaultScreenView.h"
#include "controller/DefaultScreenController.h"
#include "view/MediaFileView.h"
#include "controller/MediaFileController.h"
#include "controller/PlaylistController.h"
#include <stdexcept>

/* Private constructor for ManagerController */
ManagerController::ManagerController()
    : managerView(&ManagerView::getInstance()),
      managerModel(&ManagerModel::getInstance()) {
    try {
        /* Load playlists from file during initialization */
        managerModel->getPlaylistLibrary().loadFromFile("playlists.txt");
        cout << "Playlists loaded successfully from file.\n";
    } catch (const exception& e) {
        /* Handle errors during playlist loading */
        cerr << "Error loading playlists: " << e.what() << '\n';
    }
}

/* Get the ManagerView instance */
ManagerView* ManagerController::getManagerView() {
    return managerView;
}

/* Get the ManagerModel instance */
ManagerModel* ManagerController::getManagerModel() {
    return managerModel;
}

/* Get a controller by its key */
BaseController* ManagerController::getController(const string& key) const {
    auto it = controllers.find(key);
    if (it != controllers.end()) {
        return it->second;
    }
    /* Return nullptr if the controller is not found */
    return nullptr;
}

/* Set or update a controller with a specific key */
void ManagerController::setController(const string& key, BaseController* controller) {
    controllers[key] = controller;
}

/* Destructor to clean up dynamically allocated controllers */
ManagerController::~ManagerController() {
    for (auto& pair : controllers) {
        delete pair.second;
    }
    controllers.clear();
}

/* Static method to get the singleton instance of ManagerController */
ManagerController& ManagerController::getInstance() {
    static ManagerController instance;
    return instance;
}

/* Register a controller with a specific key */
void ManagerController::registerController(const string& key, BaseController* controller) {
    controllers[key] = controller;
}

/* Initialize the views and register controllers */
void ManagerController::initializeViews() {
    ManagerView& managerView = ManagerView::getInstance();
    // ManagerModel& managerModel = ManagerModel::getInstance();

    /* Register DefaultScreenView and DefaultScreenController */
    DefaultScreenView* defaultView = new DefaultScreenView();
    DefaultScreenController* defaultController = new DefaultScreenController();

    managerView.registerView("Default", defaultView);
    registerController("Default", defaultController);

    /* Register MediaFileView and MediaFileController */
    MediaFileView* mediaFileView = new MediaFileView();
    MediaFileController* mediaFileController = new MediaFileController();

    managerView.registerView("MediaFile", mediaFileView);
    registerController("MediaFile", mediaFileController);

    /* Register MetadataView and MetadataController */
    MetadataView* metaDataView = new MetadataView();
    MetadataController* metadataController = new MetadataController();

    managerView.registerView("Metadata", metaDataView);
    registerController("Metadata", metadataController);

    /* Register PlaylistView and PlaylistController */
    PlaylistView* playlistView = new PlaylistView();
    PlaylistController* playlistController = new PlaylistController();

    managerView.registerView("Playlist", playlistView);
    registerController("Playlist", playlistController);

    /* Set the initial view to Default */
    managerView.setView("Default");
}

/* Main loop to run the application */
void ManagerController::run() {
    ManagerView& managerView = ManagerView::getInstance();

    while (true) {
        /* Check if the current view is valid */
        if (managerView.getView() == nullptr) {
            cerr << "Error: Current view is null!" << endl;
            break;
        }

        /* Get the key of the current view */
        string currentViewKey = managerView.getCurrentViewKey();

        /* Show the menu of the current view */
        managerView.getView()->showMenu();

        /* Handle user input */
        int action = managerView.getView()->handleInput();

        /* Find the corresponding controller and handle the action */
        auto it = controllers.find(currentViewKey);
        if (it != controllers.end() && it->second != nullptr) {
            it->second->handleAction(action);
        } else {
            cerr << "Error: No controller found for view: " << currentViewKey << endl;
        }
    }
}
