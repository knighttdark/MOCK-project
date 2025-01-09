#include "controller/MetadataController.h"
#include <taglib/tag.h>
#include <taglib/audioproperties.h>

void restoreMetadata(TagLib::Tag* tag, const Metadata& originalMetadata) {
    tag->setTitle(TagLib::String(originalMetadata.getValue("Title")));
    tag->setArtist(TagLib::String(originalMetadata.getValue("Artist")));
    tag->setAlbum(TagLib::String(originalMetadata.getValue("Album")));
    tag->setYear(std::stoi(originalMetadata.getValue("Year")));
    tag->setTrack(std::stoi(originalMetadata.getValue("Track")));
    tag->setGenre(TagLib::String(originalMetadata.getValue("Genre")));
}
