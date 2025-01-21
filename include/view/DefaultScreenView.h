#ifndef DEFAULTSCREENVIEW_H
#define DEFAULTSCREENVIEW_H

#include "common/BaseView.h"
#include "common/MenuRenderer.h"
#include <iostream>

class DefaultScreenView : public BaseView {
public:
    int showMenu() override;
};

#endif /* DEFAULTSCREENVIEW_H */
