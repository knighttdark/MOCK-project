#ifndef BASE_CONTROLLER_H
#define BASE_CONTROLLER_H

using namespace std;

/* Basic interface for controllers */
class BaseController {
public:
    virtual void handleAction(int action) = 0; /* Handle action */
    virtual ~BaseController() {} /* Destructor */
};

#endif /* BASE_CONTROLLER_H */
