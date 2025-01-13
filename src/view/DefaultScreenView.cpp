#include "view/DefaultScreenView.h"
#include "view/ManagerView.h"
#include <common/TerminalUtils.h>
#include <common/Exception.h>
#include <iostream>
#include <climits>

void DefaultScreenView::showMenu() {
    clearTerminal();
    cout << "=============================\n";
    cout << " Welcome to the Default Screen\n";
    cout << "=============================\n";
    cout << "1. Browse Media Files\n";
    cout << "2. Playlist\n";
    cout << "0. Exit\n";
}

int DefaultScreenView::handleInput() {
    int choice = Exception::getValidatedInput_Int("Choose an option: ", Exception::validateInputDefaultScreen);
    return choice;
}



