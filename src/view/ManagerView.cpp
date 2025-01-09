#include "view/ManagerView.h"
#include <iostream>

// Private constructor
ManagerView::ManagerView() : currentView(nullptr), currentViewKey("") {}

// Static method to get the instance
ManagerView& ManagerView::getInstance() {
    static ManagerView instance;
    return instance;
}

void ManagerView::registerView(const std::string& key, BaseView* view) {
    views[key] = view;
}

void ManagerView::setView(const std::string& key) {
    if (views.find(key) != views.end()) {
        currentViewKey = key;
        currentView = views[key];
    }
    else {
        std::cerr << "View not found: " << key << std::endl;
    }
}

BaseView* ManagerView::getView() const {
    return currentView;
}

std::string ManagerView::getCurrentViewKey() const {
    return currentViewKey;
}

void ManagerView::switchScreen(BaseView* view) {
    currentView = view;
}
