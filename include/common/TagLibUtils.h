#ifndef TAGLIB_UTILS_H
#define TAGLIB_UTILS_H

#include <taglib/fileref.h>
#include <taglib/tag.h>
#include <taglib/audioproperties.h>
#include "Metadata.h"

void printMetadata(TagLib::Tag* tag, TagLib::AudioProperties* audioProperties);
void restoreMetadata(TagLib::Tag* tag, const Metadata& originalMetadata);

#endif // TAGLIB_UTILS_H
