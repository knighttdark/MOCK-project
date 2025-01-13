#ifndef DEFAULTSCREENVIEW_H
#define DEFAULTSCREENVIEW_H

#include "common/BaseView.h"
#include <iostream>

/* Default screen view */
class DefaultScreenView : public BaseView {
public:
    void showMenu() override; /* Show menu */
    int handleInput() override; /* Handle input */
};

#endif /* DEFAULTSCREENVIEW_H */
