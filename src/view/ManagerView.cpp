#include "view/ManagerView.h"
#include <iostream>
ManagerView* ManagerView::instance = nullptr;

ManagerView::ManagerView() : currentView(nullptr), currentViewKey("") {}
ManagerView::~ManagerView() {
    for (auto& pair : views) {
        if (pair.second) {  
            delete pair.second;
            pair.second = nullptr;  
        }
    }
}

ManagerView& ManagerView::getInstance() {
    static ManagerView instance;
    return instance;
}

void ManagerView::registerView(const string& key, BaseView* view) {
    views[key] = view;
}

void ManagerView::setView(const string& key) {
    if (views.find(key) != views.end()) {
        currentViewKey = key;
        currentView = views[key];
    }
    else {
        cerr << "View not found: " << key << endl;
    }
}

BaseView* ManagerView::getView() const {
    return currentView;
}

string ManagerView::getCurrentViewKey() const {
    return currentViewKey;
}

void ManagerView::switchScreen(BaseView* view) {
    currentView = view;
}
