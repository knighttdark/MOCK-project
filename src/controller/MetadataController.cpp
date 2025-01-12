#include "controller/MetadataController.h"
#include <iostream>
#include <climits>

void MetadataController::setCurrentTag(TagLib::Tag* tag) {
    currentTag = tag; 
}

void MetadataController::handleShowMetadata(const std::string& filepath) {
    if (filepath.empty()) {
        std::cerr << "Error: Invalid file path!" << std::endl;
        return;
    }

    currentFilePath = filepath;
    currentFileRef = TagLib::FileRef(filepath.c_str());
    if (currentFileRef.isNull()) {
        std::cerr << "Error: Unable to open file. Please check the file path!" << std::endl;
        return;
    }

    currentTag = currentFileRef.tag();
    if (!currentTag) {
        std::cerr << "Error: No metadata available in the file!" << std::endl;
        return;
    }

    if (!currentFileRef.audioProperties()) {
        std::cerr << "Error: No audio properties available for the file!" << std::endl;
        return;
    }

    // Hiển thị Metadata
    std::map<std::string, std::string> metadata = Metadata::convertTagToMap(currentTag, currentFileRef.audioProperties());
    MetadataView metadataView;
    metadataView.displayMetadata(metadata);
}


void MetadataController::handleAction(int action) {
    if (!currentTag) { // currentTag là đối tượng TagLib::Tag* đang xử lý
        std::cerr << "Error: No metadata loaded to edit!" << std::endl;
        return;
    }

    std::string newValue;
    int newYear;

    switch (action) {
    case 1: { // Edit Title
        std::cout << "Enter new title: ";
        std::getline(std::cin, newValue);
        currentTag->setTitle(TagLib::String(newValue)); // Cập nhật Title 
        saveMetadata();
        break;
    }
    case 2: { // Edit Artist
        std::cout << "Enter new artist: ";
        std::getline(std::cin, newValue);
        currentTag->setArtist(TagLib::String(newValue)); // Cập nhật Artist
        std::cout << "Artist updated successfully." << std::endl;
        saveMetadata();
        break;
    }
    case 3: { // Edit Album
        std::cout << "Enter new album: ";
        std::getline(std::cin, newValue);
        currentTag->setAlbum(TagLib::String(newValue)); // Cập nhật Album
        std::cout << "Album updated successfully." << std::endl;
        saveMetadata();
        break;
    }
    case 4: { // Edit Genre
        std::cout << "Enter new genre: ";
        std::getline(std::cin, newValue);
        currentTag->setGenre(TagLib::String(newValue)); // Cập nhật Genre
        std::cout << "Genre updated successfully." << std::endl;
        saveMetadata();
        break;
    }
    case 5: { // Edit Year
        std::cout << "Enter new year: ";
        std::cin >> newYear;
        currentTag->setYear(newYear); // Cập nhật Year
        std::cin.ignore(); // Xóa ký tự '\n' còn lại trong input
        std::cout << "Year updated successfully." << std::endl;
        saveMetadata();
        break;
    }
    case 6: { // Edit Track
        std::cout << "Enter new track number: ";
        std::cin >> newYear;
        currentTag->setTrack(newYear); // Cập nhật Track
        std::cin.ignore();
        std::cout << "Track number updated successfully." << std::endl;
        saveMetadata();
        break;
    }
    case 0: { // Exit Editing
        std::cout << "Exiting Metadata Editing..." << std::endl;
        MediaFileController* mediaFileController = dynamic_cast<MediaFileController*>(
            ManagerController::getInstance().getController("MediaFile"));
        
        if (!mediaFileController) {
            std::cerr << "Error: MediaFileController is not available!" << std::endl;
            break;
        }
        ManagerController::getInstance().getManagerView()->setView("MediaFile");
        std::cout << "\nSwitching to Media File View..." << std::endl;
        mediaFileController->scanAndDisplayMedia();
        
        return;
    }
    default:
        std::cerr << "Invalid choice! Please try again." << std::endl;
        break;
    }
    currentFileRef = TagLib::FileRef(currentFilePath.c_str());
    if (currentFileRef.isNull()) {
        std::cerr << "Error: Unable to refresh metadata!" << std::endl;
        return;
    }

    // show new metadata update
    handleShowMetadata(currentFilePath);
    
}


void MetadataController::saveMetadata() {

    if (currentFileRef.isNull()) {
        std::cerr << "Error: currentFileRef is null! Cannot save metadata." << std::endl;
        return;
    }

    if (currentFileRef.save()) {
        std::cout << "Metadata saved successfully!" << std::endl;
    } else {
        std::cerr << "Error: Could not save metadata to file." << std::endl;
    }
}
