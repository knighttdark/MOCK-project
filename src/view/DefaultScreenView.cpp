#include "view/DefaultScreenView.h"
#include "view/ManagerView.h"

int DefaultScreenView::showMenu() {
    MenuRenderer menu("Main Menu", {"1. Browse Media", "2. Playlist", "0. Exit"}, {1, 2, 0});
    return menu.render();
}


