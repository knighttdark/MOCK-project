#ifndef MEDIAFILE_H
#define MEDIAFILE_H

#include <string>
#include "Metadata.h"

/* MediaFile class to represent a media file with its properties and metadata */
class MediaFile {
private:
    int index; /* Index of the MediaFile in the list */
    string name;
    string path;
    string type;
    Metadata metadata;

public:
    /* Constructors */
    MediaFile();
    MediaFile(int index, const string& name, const string& path, const string& type);

    /* Getters and Setters */
    int getIndex() const;
    void setIndex(int newIndex);

    string getName() const;
    string getPath() const;
    string getType() const;
    Metadata getMetadata() const;
    void setMetadata(const Metadata& metadata);

    string getInfo() const;
};

#endif /* MEDIAFILE_H */