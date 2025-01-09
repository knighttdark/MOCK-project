#ifndef BASE_CONTROLLER_H
#define BASE_CONTROLLER_H

class BaseController {
public:
    virtual void handleAction(int action) = 0;
    virtual ~BaseController() {}
};

#endif 
