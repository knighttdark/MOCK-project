#ifndef BASEVIEW_H
#define BASEVIEW_H

#include <ftxui/component/component.hpp>  // For interactive components.
#include <ftxui/component/screen_interactive.hpp>  // For ScreenInteractive.
#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

using namespace ftxui;
/* Basic interface for views */
class BaseView {
public:
    // virtual void showMenu() = 0; /* Display menu */
    // virtual int handleInput() = 0; /* Handle input */
    virtual int showMenu() = 0; /* Show menu */
    virtual ~BaseView() {} /* Destructor */
};

#endif /* BASEVIEW_H */
