#ifndef BASEVIEW_H
#define BASEVIEW_H

#include <ftxui/component/component.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

using namespace ftxui;
/* Basic interface for views */
class BaseView {
public:
    virtual int showMenu() = 0; /* Show menu */
    virtual ~BaseView() {} /* Destructor */
};

#endif /* BASEVIEW_H */
