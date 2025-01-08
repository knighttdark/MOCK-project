#include "controller/MediaController.h"
#include "view/MediaView.h"
#include <iostream>

int main() {
    MediaController controller;
    MediaView view;

    std::string mediaDir = "media";
    controller.showFileList(mediaDir);

    // Further implementation for interacting with user input
}
