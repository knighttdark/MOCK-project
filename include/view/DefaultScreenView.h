#ifndef DEFAULTSCREENVIEW_HPP
#define DEFAULTSCREENVIEW_HPP

#include "common/BaseView.h"
#include <iostream>

class DefaultScreenView : public BaseView {
public:
    void showMenu() override;
    int handleInput() override;
};

#endif // DEFAULTSCREENVIEW_HPP
