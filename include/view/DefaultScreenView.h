#ifndef DEFAULTSCREENVIEW_H
#define DEFAULTSCREENVIEW_H

#include "common/BaseView.h"
#include "common/MenuRenderer.h"
#include <iostream>

/* Default screen view */
class DefaultScreenView : public BaseView {
public:

    int showMenu() override; /* Hiển thị menu */
};

#endif /* DEFAULTSCREENVIEW_H */
