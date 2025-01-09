#ifndef METADATACONTROLLER_H
#define METADATACONTROLLER_H

#include <taglib/tag.h>
#include <taglib/audioproperties.h>
#include "model/Metadata.h"

void restoreMetadata(TagLib::Tag* tag, const Metadata& originalMetadata);

#endif // METADATACONTROLLER_H
