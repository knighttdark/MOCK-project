#include "controller/DefaultScreenController.h"

DefaultScreenController::DefaultScreenController(ManagerView* managerView) : manager(managerView) {}



void DefaultScreenController::handleAction(int action) {
    switch (action) {
    case 1:
        system("cls");
        std::cout << "\nSwitching to Media File View..." << std::endl;
        manager->setView("MediaFile");
        break;
    case 2:
        std::cout << "\nSwitching to Playlist View..." << std::endl;
        manager->setView("Playlist");
        break;
    case 3:
        std::cout << "\nSwitching to Now Playing View..." << std::endl;
        manager->setView("NowPlaying");
        break;
    case 4:
        std::cout << "\nExiting the application..." << std::endl;
        exit(0);
    default:
        std::cout << "Invalid choice! Please try again." << std::endl;
    }
}
