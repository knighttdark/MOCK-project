#ifndef BASEVIEW_H
#define BASEVIEW_H

using namespace std;

/* Basic interface for views */
class BaseView {
public:
    virtual void showMenu() = 0; /* Display menu */
    virtual int handleInput() = 0; /* Handle input */
    virtual ~BaseView() {} /* Destructor */
};

#endif /* BASEVIEW_H */
