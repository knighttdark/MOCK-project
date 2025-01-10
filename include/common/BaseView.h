#ifndef BASEVIEW_H
#define BASEVIEW_H

class BaseView {
public:
    virtual void showMenu() = 0;
    virtual int handleInput() = 0;
    virtual ~BaseView() {}
};

#endif
