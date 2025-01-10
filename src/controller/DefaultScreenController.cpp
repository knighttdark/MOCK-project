#include "controller/DefaultScreenController.h"

DefaultScreenController::DefaultScreenController(){}



void DefaultScreenController::handleAction(int action) {
    switch (action) {
    case 1:
        system("clear");
        std::cout << "\nSwitching to Media File View..." << std::endl;
        ManagerController::getInstance().getManagerView()->setView("MediaFile");
        break;
    case 2:
        std::cout << "\nSwitching to Playlist View..." << std::endl;
        ManagerController::getInstance().getManagerView()->setView("Playlist");
        break;
    case 3:
        std::cout << "\nSwitching to Now Playing View..." << std::endl;
        ManagerController::getInstance().getManagerView()->setView("NowPlaying");
        break;
    case 4:
        std::cout << "\nExiting the application..." << std::endl;
        exit(0);
    default:
        std::cout << "Invalid choice! Please try again." << std::endl;
    }
}
