#ifndef MANAGER_VIEW_H
#define MANAGER_VIEW_H

#include <unordered_map>
#include <string>
#include "common/BaseView.h"

class ManagerView {
private:
    unordered_map<string, BaseView*> views;
    BaseView* currentView;
    string currentViewKey;
    ManagerView();
public:
    ManagerView(const ManagerView&) = delete;
    ManagerView& operator=(const ManagerView&) = delete;
    static ManagerView& getInstance();
    void registerView(const string& key, BaseView* view);
    void setView(const string& key);
    BaseView* getView() const;
    string getCurrentViewKey() const;
    void switchScreen(BaseView* view);
    ~ManagerView();
};

#endif 
