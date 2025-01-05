#include "./model/MediaFile.h"

MediaFile::MediaFile(const std::string &path, const std::string &type)
    : filePath(path), fileType(type), metadata("Default Metadata") {}

std::string MediaFile::getName() const
{
    return filePath;
}

std::string MediaFile::getMetadata() const
{
    return metadata;
}

void MediaFile::editMetadata(const std::string &key, const std::string &value)
{
    metadata = key + ": " + value;
}
