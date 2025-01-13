#include "common/TerminalUtils.h"
#include <cstdlib> /* For system() */

void clearTerminal() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}
