#include <iostream>
#include "view/PrintMetadata.h"
#include "controller/MetadataController.h"
#include "model/Metadata.h"
#include <taglib/fileref.h>
#include "common/FileUtils.h"

int main() {
    const std::string mediaDir = "media";
    int choice;
    do {
        std::cout << "\nChoose an option:" << std::endl;
        std::cout << "1. Show All Files in Media Folder" << std::endl;
        std::cout << "0. Exit" << std::endl;
        std::cout << "Enter your choice: ";
        std::cin >> choice;
        std::cin.ignore();  

        if (choice == 1) {
            showAllFiles(mediaDir);

            std::cout << "\nEnter the number of the file you want to edit: ";
            int fileChoice;
            std::cin >> fileChoice;
            std::cin.ignore();  

            int currentIndex = 1;
            std::string filepath;
            for (const auto& entry : fs::directory_iterator(mediaDir)) {
                if (fs::is_regular_file(entry)) {
                    if (currentIndex == fileChoice) {
                        filepath = entry.path().string();
                        break;
                    }
                    currentIndex++;
                }
            }

            if (!filepath.empty()) {
                TagLib::FileRef f(filepath.c_str());

                if (!f.isNull() && f.tag() && f.audioProperties()) {
                    TagLib::Tag* tag = f.tag();
                    TagLib::AudioProperties* audioProperties = f.audioProperties();

                    Metadata originalMetadata;
                    originalMetadata.setValue("Title", tag->title().toCString());
                    originalMetadata.setValue("Artist", tag->artist().toCString());
                    originalMetadata.setValue("Album", tag->album().toCString());
                    originalMetadata.setValue("Year", std::to_string(tag->year()));
                    originalMetadata.setValue("Track", std::to_string(tag->track()));
                    originalMetadata.setValue("Genre", tag->genre().toCString());

                    printMetadata(tag, audioProperties);

                    int subChoice;
                    do {
                        std::cout << "\nChoose an option to edit metadata:" << std::endl;
                        std::cout << "1. Edit Title" << std::endl;
                        std::cout << "2. Edit Artist" << std::endl;
                        std::cout << "3. Edit Album" << std::endl;
                        std::cout << "4. Edit Genre" << std::endl;
                        std::cout << "5. Edit Year" << std::endl;
                        std::cout << "6. Edit Track" << std::endl;
                        std::cout << "7. Restore Original Values" << std::endl;
                        std::cout << "0. Exit Editing" << std::endl;
                        std::cout << "Enter your choice: ";
                        std::cin >> subChoice;
                        std::cin.ignore();  

                        std::string newValue;
                        unsigned int newYear;
                        switch (subChoice) {
                            case 1:
                                std::cout << "Enter new title: ";
                                std::getline(std::cin, newValue);
                                tag->setTitle(newValue.c_str());  // Đảm bảo giá trị mới được gán vào tag
                                originalMetadata.setValue("Title", newValue);
                                break;

                            case 2:
                                std::cout << "Enter new artist: ";
                                std::getline(std::cin, newValue);
                                tag->setArtist(newValue.c_str());  // Cập nhật giá trị mới vào artist
                                originalMetadata.setValue("Artist", newValue);
                                break;

                            case 3:
                                std::cout << "Enter new album: ";
                                std::getline(std::cin, newValue);
                                tag->setAlbum(newValue.c_str());  // Cập nhật giá trị mới vào album
                                originalMetadata.setValue("Album", newValue);
                                break;

                            case 4:
                                std::cout << "Enter new genre: ";
                                std::getline(std::cin, newValue);
                                tag->setGenre(newValue.c_str());  // Cập nhật giá trị mới vào genre
                                originalMetadata.setValue("Genre", newValue);
                                break;

                            case 5:
                                std::cout << "Enter new year: ";
                                std::cin >> newYear;
                                std::cin.ignore();
                                tag->setYear(newYear);  // Cập nhật giá trị mới vào year
                                originalMetadata.setValue("Year", std::to_string(newYear));
                                break;

                            case 6:
                                std::cout << "Enter new track number: ";
                                std::cin >> newYear;
                                std::cin.ignore();
                                tag->setTrack(newYear);  // Cập nhật giá trị mới vào track
                                originalMetadata.setValue("Track", std::to_string(newYear));
                                break;

                            case 7:
                                restoreMetadata(tag, originalMetadata);  // Khôi phục giá trị gốc
                                break;

                            default:
                                break;
                        }

                        if (subChoice != 0) {
                            printMetadata(tag, audioProperties);  // In lại thông tin metadata sau khi chỉnh sửa
                        }

                    } while (subChoice != 0);

                    // Sau khi cập nhật metadata, lưu các thay đổi vào file
                    if (f.save()) {
                        std::cout << "\nMetadata updated and saved successfully!" << std::endl;
                    } else {
                        std::cerr << "Error: Could not save changes to the file." << std::endl;
                    }
                } else {
                    std::cerr << "Error: Could not open file or retrieve metadata." << std::endl;
                }
            } else {
                std::cerr << "Invalid file choice." << std::endl;
            }
        }

    } while (choice != 0);

    return 0;
}
