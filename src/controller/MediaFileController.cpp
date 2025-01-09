#include "controller/MediaFileController.h"

MediaFileController::MediaFileController(ManagerView* managerView) : manager(managerView) {}

void MediaFileController::handleAction(int action) {
    switch (action) {
    case 1:
        //system("cls");
        std::cout << "\nShowing Metadata..." << std::endl;
        system("pause");
        break;
    case 2:
        //system("cls");
        std::cout << "\nEditing Metadata..." << std::endl;
        break;
    case 3:
        //system("cls");
        std::cout << "\nGoing to Next Page..." << std::endl;
        break;
    case 4:
        //system("cls");
        std::cout << "\nGoing to Previous Page..." << std::endl;
        break;
    case 5:
        //system("cls");
        std::cout << "\nPlaying Media by ID..." << std::endl;
        break;
    case 6:
        std::cout << "\nReturning Home..." << std::endl;
        system("cls");
        manager->setView("Default");
        break;
    default:
        std::cout << "Invalid choice! Please try again." << std::endl;
        break;
    }
}
