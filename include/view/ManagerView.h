#ifndef MANAGER_VIEW_H
#define MANAGER_VIEW_H

#include <unordered_map>
#include <string>
#include "common/BaseView.h"

class ManagerView {
private:
    std::unordered_map<std::string, BaseView*> views;
    BaseView* currentView;
    std::string currentViewKey;

    // Private constructor for Singleton
    ManagerView();

public:
    // Delete copy constructor and assignment operator
    ManagerView(const ManagerView&) = delete;
    ManagerView& operator=(const ManagerView&) = delete;

    // Static method to get the instance
    static ManagerView& getInstance();

    void registerView(const std::string& key, BaseView* view);
    void setView(const std::string& key);
    BaseView* getView() const;
    std::string getCurrentViewKey() const;
    void switchScreen(BaseView* view);
    ~ManagerView();
};

#endif // MANAGER_VIEW_HPP
