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

class MetadataController : public BaseController {
private:
    TagLib::Tag* currentTag = nullptr;
    TagLib::FileRef currentFileRef;
public:

    void handleShowMetadata(const std::string& filepath);
    void handleAction(int action) override;
    void setCurrentTag(TagLib::Tag* tag);
    void saveMetadata();
};

#endif // METADATACONTROLLER_H
