#ifndef MEDIAFILE_H
#define MEDIAFILE_H

#include <string>
#include "Metadata.h"

using namespace std;

class MediaFile {
private:
    string name;
    string path;
    string type;
    Metadata metadata;

public:
    // Constructors
    MediaFile() = default;
    MediaFile(const string& name, const string& path, const string& type);

    // Getters and Setters
    string getName() const;
    string getPath() const;
    string getType() const;
    Metadata getMetadata() const;
    void setMetadata(const Metadata& metadata);

    // Overload toán tử ==
    bool operator==(const MediaFile& other) const;
};

#endif // MEDIAFILE_HPP
