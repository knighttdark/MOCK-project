
#include "controller/MediaFileController.h"
#include <stdexcept>
#include <iostream>

MediaFileController::MediaFileController(){}


void MediaFileController::scanDirectory(const std::string& path) {
    try {
        ManagerController::getInstance().getManagerModel()->getMediaLibrary().scanDirectory(path);
        std::cout << "Directory scanned successfully.\n";
    } catch (const std::exception& e) {
        std::cerr << "Error scanning directory: " << e.what() << std::endl;
    }
}


void MediaFileController::nextPage() {
    MediaFileView* mediaFileView = dynamic_cast<MediaFileView*>(ManagerController::getInstance().getManagerView()->getView());
    if (!mediaFileView) {
        std::cerr << "Error: MediaFileView is null!" << std::endl;
        return;
    }

    auto& mediaLibrary =  ManagerController::getInstance().getManagerModel()->getMediaLibrary();
    if (currentPage + 1 < mediaLibrary.getTotalPages(pageSize)) {
        currentPage++;
        auto files = mediaLibrary.getMediaFilesForPage(currentPage, pageSize);
        std::vector<std::string> fileStrings;
        for (const auto& file : files) {
            fileStrings.push_back(std::to_string(file.getIndex()) + ". " + file.getName());
        }

        mediaFileView->displayMediaFiles(fileStrings, currentPage + 1);
        mediaFileView->displayPagination(currentPage + 1, mediaLibrary.getTotalPages(pageSize));
    } else {
        std::cout << "Already on the last page.\n";
    }
}

void MediaFileController::previousPage() {
    MediaFileView* mediaFileView = dynamic_cast<MediaFileView*>(ManagerController::getInstance().getManagerView()->getView());
    if (!mediaFileView) {
        std::cerr << "Error: MediaFileView is null!" << std::endl;
        return;
    }

    auto& mediaLibrary = ManagerController::getInstance().getManagerModel()->getMediaLibrary();
    if (currentPage > 0) {
        currentPage--;
        auto files = mediaLibrary.getMediaFilesForPage(currentPage, pageSize);
        std::vector<std::string> fileStrings;
        for (const auto& file : files) {
            fileStrings.push_back(std::to_string(file.getIndex()) + ". " + file.getName());
        }

        mediaFileView->displayMediaFiles(fileStrings, currentPage + 1);
        mediaFileView->displayPagination(currentPage + 1, mediaLibrary.getTotalPages(pageSize));
    } else {
        std::cout << "Already on the first page.\n";
    }
}


void MediaFileController::scanAndDisplayMedia() {
    MediaFileView* mediaFileView = dynamic_cast<MediaFileView*>(ManagerController::getInstance().getManagerView()->getView());
    if (!mediaFileView) {
        std::cerr << "Error: MediaFileView is null!" << std::endl;
        return;
    }

    // Access media library and initialize pagination
    auto& mediaLibrary = ManagerController::getInstance().getManagerModel()->getMediaLibrary();
    int totalPages = mediaLibrary.getTotalPages(pageSize);

    // Get MediaFile objects and convert to strings for display
    auto files = mediaLibrary.getMediaFilesForPage(0, pageSize);
    std::vector<std::string> fileStrings;

    for (const auto& file : files) {
    fileStrings.push_back(std::to_string(file.getIndex()) + ". " + file.getName());
    }

    // Display first page of media files
    mediaFileView->displayMediaFiles(fileStrings, 1);
    mediaFileView->displayPagination(1, mediaLibrary.getTotalPages(pageSize));
    // Exit after displaying the first page
    return;
}



void MediaFileController::handleAction(int action) {
    switch (action) {
    case 1:
        std::cout << "\nShowing Metadata..." << std::endl;
        //mediaFileView->showMetadata(managerModel->getMediaLibrary().getMetadata());
        break;
    case 2:
        std::cout << "\nEditing Metadata..." << std::endl;
        //mediaFileView->editMetadata(managerModel->getMediaLibrary().getMetadata());
        break;
    case 3:
        std::cout << "\nGoing to Next Page..." << std::endl;
        nextPage();
        break;
    case 4:
        std::cout << "\nGoing to Previous Page..." << std::endl;
        previousPage();
        break;
    case 5:
        int mediaId;
        std::cout << "\nEnter Media ID to Play: ";
        std::cin >> mediaId;
        std::cout << "\nPlaying Media with ID: " << mediaId << std::endl;
        //model->getMediaLibrary().playMedia(mediaId);
        break;
    case 6:
        std::cout << "\nReturning Home...\n";
        ManagerController::getInstance().getManagerView()->setView("Default");
        //system("clear");
        break;
    default:
        std::cout << "Invalid choice! Please try again." << std::endl;
        break;
    }
}

