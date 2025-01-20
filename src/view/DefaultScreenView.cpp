#include "view/DefaultScreenView.h"
#include "view/ManagerView.h"

// void DefaultScreenView::showMenu() {
//     clearTerminal();
//     cout << "=============================\n";
//     cout << " Welcome to the Default Screen\n";
//     cout << "=============================\n";
//     cout << "1. Browse Media Files\n";
//     cout << "2. Playlist\n";
//     cout << "0. Exit\n";
// }

// int DefaultScreenView::handleInput() {
//     int choice = Exception::getValidatedInput_Int("Choose an option: ", Exception::validateInputDefaultScreen);
//     return choice;
// }


int DefaultScreenView::showMenu() {
    MenuRenderer menu("Main Menu", {"1. Browse Media", "2. Playlist", "0. Exit"}, {1, 2, 0});
    return menu.render();
}


