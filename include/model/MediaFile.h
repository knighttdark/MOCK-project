#ifndef MEDIAFILE_H
#define MEDIAFILE_H

#include <string>

class MediaFile
{
public:
    MediaFile(const std::string &path, const std::string &type);
    std::string getName() const;
    std::string getMetadata() const;
    void editMetadata(const std::string &key, const std::string &value);

private:
    std::string filePath;
    std::string fileType;
    std::string metadata;
};

#endif
