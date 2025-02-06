#ifndef METADATACONTROLLER_H
#define METADATACONTROLLER_H

#include <taglib/fileref.h>
#include <taglib/tag.h>
#include "model/Metadata.h"
#include "view/MetadataView.h"
#include "model/MediaFile.h"
#include "common/BaseController.h"
#include "controller/ManagerController.h"
#include "controller/MediaFileController.h"
#include "view/MediaFileView.h"

/* Manage metadata actions */
class MetadataController : public BaseController {
private:
    TagLib::Tag* currentTag = nullptr;
    TagLib::FileRef currentFileRef;
    string currentFilePath;

public:
    virtual void handleShowMetadata(const string& filepath);
    void handleAction(int action) override;
    void setCurrentTag(TagLib::Tag* tag);
    void saveMetadata();
    void handleEditAction(const string& field_name, const string& placeholder, function<void(const string&)> updateField);
};

#endif
