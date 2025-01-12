#include "view/DefaultScreenView.h"
#include "view/ManagerView.h"
#include <iostream>
#include <climits>
#include <common/TerminalUtils.h>

void DefaultScreenView::showMenu() {
    clearTerminal();
    std::cout << "=============================\n";
    std::cout << " Welcome to the Default Screen\n";
    std::cout << "=============================\n";
    std::cout << "1. Browse Media Files\n";
    std::cout << "2. Playlist\n";
    std::cout << "0. Exit\n";
}

int DefaultScreenView::handleInput() {
    int choice;
    std::cout << "\nChoose an option: ";
    std::cin >> choice;
    std::cin.ignore(INT_MAX, '\n');
    return choice;
}


