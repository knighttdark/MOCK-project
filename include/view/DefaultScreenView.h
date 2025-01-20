#ifndef DEFAULTSCREENVIEW_H
#define DEFAULTSCREENVIEW_H

#include "common/BaseView.h"
#include "common/MenuRenderer.h"
#include <iostream>

/* Default screen view */
class DefaultScreenView : public BaseView {
public:
    // void showMenu() override; /* Show menu */
    // int handleInput() override; /* Handle input */
    int showMenu() override; /* Hiển thị menu */
};

#endif /* DEFAULTSCREENVIEW_H */
// #ifndef DEFAULT_SCREEN_VIEW_H
// #define DEFAULT_SCREEN_VIEW_H

// #include "common/BaseView.h"
// #include <iostream>
// #include "common/MenuRenderer.h"

// // DefaultScreenView kế thừa từ BaseView
// class DefaultScreenView : public BaseView {
// private:
//     MenuRenderer menu; // Đối tượng MenuRenderer quản lý menu

// public:
//     // Constructor
//     DefaultScreenView();

//     // Phương thức hiển thị menu (override từ BaseView)
//     int showMenu() override;
// };

// #endif // DEFAULT_SCREEN_VIEW_H
