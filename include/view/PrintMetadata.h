#ifndef PRINTMETADATA_H
#define PRINTMETADATA_H

#include <taglib/tag.h>
#include <taglib/audioproperties.h>

void printMetadata(TagLib::Tag* tag, TagLib::AudioProperties* audioProperties);
void showAllFiles(const std::string& directory);

#endif // PRINTMETADATA_H
