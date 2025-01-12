
#include "controller/MediaFileController.h"
#include <stdexcept>
#include <iostream>

MediaFileController::MediaFileController(){}


void MediaFileController::scanDirectory(const string& path) {
    try {
        ManagerController::getInstance().getManagerModel()->getMediaLibrary().scanDirectory(path);
        cout << "Directory scanned successfully.\n";
    } catch (const exception& e) {
        cerr << "Error scanning directory: " << e.what() << endl;
    }
}


void MediaFileController::nextPage() {
    MediaFileView* mediaFileView = dynamic_cast<MediaFileView*>(ManagerController::getInstance().getManagerView()->getView());
    if (!mediaFileView) {
        cerr << "Error: MediaFileView is null!" << endl;
        return;
    }

    auto& mediaLibrary =  ManagerController::getInstance().getManagerModel()->getMediaLibrary();
    if (currentPage + 1 < mediaLibrary.getTotalPages(pageSize)) {
        currentPage++;
        auto files = mediaLibrary.getMediaFilesForPage(currentPage, pageSize);
        vector<string> fileStrings;
        for (const auto& file : files) {
            fileStrings.push_back(to_string(file.getIndex()) + ". " + file.getName());
        }

        mediaFileView->displayMediaFiles(fileStrings, currentPage + 1);
        mediaFileView->displayPagination(currentPage + 1, mediaLibrary.getTotalPages(pageSize));
    } else {
        cout << "Already on the last page.\n";
    }
}

void MediaFileController::previousPage() {
    MediaFileView* mediaFileView = dynamic_cast<MediaFileView*>(ManagerController::getInstance().getManagerView()->getView());
    if (!mediaFileView) {
        cerr << "Error: MediaFileView is null!" << endl;
        return;
    }

    auto& mediaLibrary = ManagerController::getInstance().getManagerModel()->getMediaLibrary();
    if (currentPage > 0) {
        currentPage--;
        auto files = mediaLibrary.getMediaFilesForPage(currentPage, pageSize);
        vector<string> fileStrings;
        for (const auto& file : files) {
            fileStrings.push_back(to_string(file.getIndex()) + ". " + file.getName());
        }

        mediaFileView->displayMediaFiles(fileStrings, currentPage + 1);
        mediaFileView->displayPagination(currentPage + 1, mediaLibrary.getTotalPages(pageSize));
    } else {
        cout << "Already on the first page.\n";
    }
}


void MediaFileController::scanAndDisplayMedia() {
    MediaFileView* mediaFileView = dynamic_cast<MediaFileView*>(ManagerController::getInstance().getManagerView()->getView());
    if (!mediaFileView) {
        cerr << "Error: MediaFileView is null!" << endl;
        return;
    }

    // Access media library and initialize pagination
    auto& mediaLibrary = ManagerController::getInstance().getManagerModel()->getMediaLibrary();
    int totalPages = mediaLibrary.getTotalPages(pageSize);

    // Get MediaFile objects and convert to strings for display
    auto files = mediaLibrary.getMediaFilesForPage(0, pageSize);
    vector<string> fileStrings;

    for (const auto& file : files) {
    fileStrings.push_back(to_string(file.getIndex()) + ". " + file.getName());
    }

    // Display first page of media files
    mediaFileView->displayMediaFiles(fileStrings, 1);
    mediaFileView->displayPagination(1, mediaLibrary.getTotalPages(pageSize));
    // Exit after displaying the first page
    return;
}



void MediaFileController::handleAction(int action) {
    switch (static_cast<MediaAction>(action)) {
    case SHOW_METADATA:
        cout << "\nShowing Metadata..." << endl;
        //mediaFileView->showMetadata(managerModel->getMediaLibrary().getMetadata());
        break;
    case EDIT_METADATA:
        cout << "\nEditing Metadata..." << endl;
        //mediaFileView->editMetadata(managerModel->getMediaLibrary().getMetadata());
        break;
    case NEXT_PAGE:
        cout << "\nGoing to Next Page..." << endl;
        nextPage();
        break;
    case PREVIOUS_PAGE:
        cout << "\nGoing to Previous Page..." << endl;
        previousPage();
        break;
    case PLAY_MEDIA:
        int mediaId;
        cout << "\nEnter Media ID to Play: ";
        cin >> mediaId;
        cout << "\nPlaying Media with ID: " << mediaId << endl;
        //model->getMediaLibrary().playMedia(mediaId);
        break;
    case RETURN_HOME:
        cout << "\nReturning Home...\n";
        ManagerController::getInstance().getManagerView()->setView("Default");
        //system("clear");
        break;
    default:
        cout << "Invalid choice! Please try again." << endl;
        break;
    }
}

