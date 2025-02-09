#ifndef METADATAVIEW_H
#define METADATAVIEW_H

#include "common/BaseView.h"
#include <map>
#include <string>
#include <taglib/fileref.h>
#include <taglib/tag.h>
#include <taglib/audioproperties.h>
#include <filesystem>
#include "common/MenuRenderer.h"

class MetadataView : public BaseView {
public:
    virtual int showMenu() override;
    virtual void displayMetadata(const map<string, string>& metadata);
};

#endif
