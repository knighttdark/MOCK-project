#ifndef METADATAVIEW_H
#define METADATAVIEW_H

#include "common/BaseView.h"
#include <map>
#include <string>
#include <taglib/fileref.h>
#include <taglib/tag.h>
#include <taglib/audioproperties.h>
#include <filesystem>

/* View for displaying metadata */
class MetadataView : public BaseView {
public:
    void showMenu() override; /* Show menu */
    int handleInput() override; /* Handle input */
    void displayMetadata(const map<string, string>& metadata); /* Display metadata */
};

#endif /* METADATAVIEW_H */