#ifndef MANAGER_VIEW_H
#define MANAGER_VIEW_H

#include <unordered_map>
#include <string>
#include "common/BaseView.h"

/* Manage views and screen transitions */
class ManagerView {
private:
    unordered_map<string, BaseView*> views; /* Registered views */
    BaseView* currentView; /* Current view */
    string currentViewKey; /* Current view key */

    ManagerView(); /* Private constructor */

public:
    ManagerView(const ManagerView&) = delete; /* No copy */
    ManagerView& operator=(const ManagerView&) = delete; /* No assign */

    static ManagerView& getInstance(); /* Get instance */

    void registerView(const string& key, BaseView* view); /* Register a view */
    void setView(const string& key); /* Set current view */
    BaseView* getView() const; /* Get current view */
    string getCurrentViewKey() const; /* Get current view key */
    void switchScreen(BaseView* view); /* Switch screen */
    ~ManagerView(); /* Destructor */
};

#endif /* MANAGER_VIEW_H */