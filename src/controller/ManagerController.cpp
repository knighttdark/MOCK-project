#include "controller/ManagerController.h"
#include <iostream>
#include "view/DefaultScreenView.h"
#include "controller/DefaultScreenController.h"
#include "view/MediaFileView.h"
#include "controller/MediaFileController.h"

// Private constructor
ManagerController::ManagerController() {}

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

    // Register DefaultScreenView and DefaultScreenController
    DefaultScreenView* defaultView = new DefaultScreenView();
    DefaultScreenController* defaultController = new DefaultScreenController(&managerView);

    managerView.registerView("Default", defaultView);
    registerController("Default", defaultController);

    // Register MediaFileView and MediaFileController
    MediaFileView* mediaFileView = new MediaFileView();
    MediaFileController* mediaFileController = new MediaFileController(&managerView);

    managerView.registerView("MediaFile", mediaFileView);
    registerController("MediaFile", mediaFileController);

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

        managerView.getView()->showMenu();
        int action = managerView.getView()->handleInput();

        std::string currentViewKey = managerView.getCurrentViewKey();
        auto it = controllers.find(currentViewKey);

        if (it != controllers.end() && it->second != nullptr) {
            it->second->handleAction(action);
        }
        else {
            std::cerr << "Error: No controller found for view: " << currentViewKey << std::endl;
        }
    }
}


// void ManagerController::run() {
//     // Get the singleton instance of ManagerView
//     ManagerView& managerView = ManagerView::getInstance();

//     // Infinite loop to keep the application running until an exit condition is met
//     while (true) {
//         // Check if the current view is null
//         if (managerView.getView() == nullptr) {
//             std::cerr << "Error: Current view is null!" << std::endl;
//             break; // Exit the loop if there's no valid view
//         }

//         // Display the menu of the current view and handle user input
//         managerView.getView()->showMenu();
//         int action = managerView.getView()->handleInput();

//         // Get the current view's key to find the corresponding controller
//         std::string currentViewKey = managerView.getCurrentViewKey();
//         auto it = controllers.find(currentViewKey);

//         // Check if a controller exists for the current view key
//         if (it != controllers.end() && it->second != nullptr) {
//             // Special handling for MediaFile
//             if (currentViewKey == "MediaFile") {
//                 std::string filePath;
//                 bool isValid = false;

//                 // Keep prompting the user for a valid file path until it is provided
//                 while (!isValid) {
//                     std::cout << "Enter file path: ";
//                     std::cin >> filePath;

//                     // Validate the file path using the static method from MediaFileController
//                     if (MediaFileController::isValidFilePath(filePath)) {
//                         isValid = true;
//                         std::cout << "File path is valid: " << filePath << std::endl;

//                         // Set the valid file path in the MediaFileController
//                         MediaFileController* mediaController = dynamic_cast<MediaFileController*>(it->second);
//                         if (mediaController != nullptr) {
//                             mediaController->setFilePath(filePath);
//                         }
//                     } else {
//                         // Display an error message if the file path is invalid
//                         std::cerr << "Invalid file path. Please try again." << std::endl;
//                     }
//                 }
//             }

//             // Handle the user action with the corresponding controller
//             it->second->handleAction(action);
//         } else {
//             // Error message if no controller is found for the current view
//             std::cerr << "Error: No controller found for view: " << currentViewKey << std::endl;
//         }
//     }
// }
