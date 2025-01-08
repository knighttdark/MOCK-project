#ifndef MEDIA_VIEW_H
#define MEDIA_VIEW_H

#include <string>

class MediaView {
public:
    void displayMetadata(const std::string& filepath);
    void showError(const std::string& message);
};

#endif // MEDIA_VIEW_H
