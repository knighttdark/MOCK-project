#ifndef METADATAVIEW_H
#define METADATAVIEW_H

#include "common/BaseView.h"
#include <map>
#include <string>
#include <taglib/fileref.h>
#include <taglib/tag.h>
#include <taglib/audioproperties.h>
#include <filesystem>  
#include <map>

class MetadataView : public BaseView {
public:
    void showMenu() override;
    int handleInput() override;
    void displayMetadata(const std::map<std::string, std::string>& metadata);
};

#endif // METADATAVIEW_HPP
