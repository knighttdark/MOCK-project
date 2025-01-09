#include "view/DefaultScreenView.h"
#include "view/ManagerView.h"
#include <iostream>

void DefaultScreenView::showMenu() {
    //system("cls");
    std::cout << "=============================\n";
    std::cout << " Welcome to the Default Screen\n";
    std::cout << "=============================\n";
    std::cout << "1. Browse Media Files\n";
    std::cout << "2. Playlist\n";
    std::cout << "3. Now Playing\n";
    std::cout << "4. Exit\n";
}

int DefaultScreenView::handleInput() {
    int choice;
    std::cout << "\nChoose an option: ";
    std::cin >> choice;
    return choice;
}


//// Constructor
//DefaultScreenView::DefaultScreenView(ManagerView* managerView) : manager(managerView) {}
//
//DefaultScreenView::DefaultScreenView() {};
//

//
//void DefaultScreenView::handleInput() {
//    int choice;
//    do {
//        std::cout << "\nChoose an option: ";
//        std::cin >> choice;
//
//        switch (choice) {
//        case 1:
//            system("cls");
//            std::cout << "\nSwitching to Media File View..." << std::endl;
//            manager->switchScreen("MediaFile");
//            return;
//        case 2:
//            std::cout << "\nSwitching to Playlist View..." << std::endl;
//            manager->switchScreen("Playlist");
//            break;
//        case 3:
//            std::cout << "\nSwitching to Now Playing View..." << std::endl;
//            manager->switchScreen("NowPlaying");
//            break;
//        case 4:
//            std::cout << "\nExiting the application..." << std::endl;
//            return;
//        default:
//            std::cout << "Invalid choice! Please try again." << std::endl;
//        }
//        //showMenu()
//    } while (choice != 4);
//}
