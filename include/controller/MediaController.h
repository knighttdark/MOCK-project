#ifndef MEDIA_CONTROLLER_H
#define MEDIA_CONTROLLER_H

#include <string>

class MediaController {
public:
    void showFileList(const std::string& directory);
    void editMetadata(const std::string& filepath);
};

#endif // MEDIA_CONTROLLER_H
