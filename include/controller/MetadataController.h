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
    TagLib::Tag* currentTag = nullptr; /* Current tag */
    TagLib::FileRef currentFileRef;    /* Current file */
    string currentFilePath;            /* File path */

public:
    void handleShowMetadata(const string& filepath); /* Show metadata */
    void handleAction(int action) override;          /* Handle actions */
    void setCurrentTag(TagLib::Tag* tag);           /* Set current tag */
    void saveMetadata();                             /* Save metadata */
};

#endif /* METADATACONTROLLER_H */