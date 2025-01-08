#ifndef TAGMANAGER_H
#define TAGMANAGER_H

#include <tag.h>
#include <fileref.h>
#include <string>

class TagManager {
public:
    TagManager(const std::string& filePath);
    ~TagManager();

    std::string getTitle() const;
    std::string getArtist() const;
    std::string getAlbum() const;

private:
    TagLib::FileRef file;
};

#endif
