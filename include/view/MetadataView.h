#ifndef METADATAVIEW_H
#define METADATAVIEW_H

#include "common/BaseView.h"
#include <map>
#include <string>

class MetadataView : public BaseView {
public:
    void showMenu() override;
    int handleInput() override;
    void displayMetadata(const map<string, string>& metadata);
};

#endif // METADATAVIEW_HPP
