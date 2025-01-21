#ifndef MEDIAFILE_H
#define MEDIAFILE_H

#include <string>
#include "Metadata.h"
#include <algorithm>

class MediaFile {
private:
    int index;
    string name;
    string path;
    string type;
    Metadata metadata;
    int duration;

public:
    MediaFile();
    MediaFile(int index, const string& name, const string& path, const string& type);

    int getIndex() const;
    void setIndex(int newIndex);
    string getName() const;
    string getPath() const;
    string getType() const;
    Metadata getMetadata() const;
    void setMetadata(const Metadata& metadata);
    string getInfo() const;
    int getDuration() const;
    bool isAudio() const;
    bool isVideo() const;
};

#endif
