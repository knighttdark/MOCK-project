#ifndef MEDIAFILE_H
#define MEDIAFILE_H

#include <string>
#include "Metadata.h"

class MediaFile {
private:
    std::string filePath;
    Metadata metadata;

public:
    MediaFile(const std::string& filePath, const Metadata& metadata);
    MediaFile(int id, const std::string& name, const std::string& artist, const char* genre);

    const std::string& getFilePath() const;
    const Metadata& getMetadata() const;
    std::string getName() const;
    void displayInfo() const;
};

#endif // MEDIAFILE_H
