#ifndef MANAGER_VIEW_H
#define MANAGER_VIEW_H

#include <unordered_map>
#include <string>
#include "common/BaseView.h"

class ManagerView {
private:
    static ManagerView* instance;
    unordered_map<string, BaseView*> views;
    BaseView* currentView;
    string currentViewKey;
    ManagerView();
public:

    friend class DefaultScreenControllerTest; // ✅ Cho phép test sửa instance
    friend class MockManagerView;
    ManagerView(const ManagerView&) = delete;
    ManagerView& operator=(const ManagerView&) = delete;
    static ManagerView& getInstance();
    void registerView(const string& key, BaseView* view);
    virtual void setView(const string& key);
    virtual BaseView* getView() const;
    virtual string getCurrentViewKey() const;
    void switchScreen(BaseView* view);
    // ✅ Cho phép thay thế instance trong test
    static void setInstance(ManagerView* mockInstance) {
        instance = mockInstance;
    }
    ~ManagerView();
};

#endif 
