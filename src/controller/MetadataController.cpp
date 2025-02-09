#include "controller/MetadataController.h"
#include "common/Enum.h"
#include "common/TerminalUtils.h"
#include <iostream>
#include <climits>

/* Sets the current tag object */
// void MetadataController::setCurrentTag(TagLib::Tag* tag) {
//     currentTag = tag;
// }

// /* Handles displaying metadata of a file */
// void MetadataController::handleShowMetadata(const string& filepath) {
//     if (filepath.empty()) {
//         cerr << "Error: Invalid file path!" << endl;
//         return;
//     }

//     /* Load the file and its metadata */
//     currentFilePath = filepath;
//     currentFileRef = TagLib::FileRef(filepath.c_str());
//     if (currentFileRef.isNull()) {
//         cerr << "Error: Unable to open file. Please check the file path!" << endl;
//         return;
//     }

//     currentTag = currentFileRef.tag();
//     if (!currentTag) {
//         cerr << "Error: No metadata available in the file!" << endl;
//         return;
//     }

//     if (!currentFileRef.audioProperties()) {
//         cerr << "Error: No audio properties available for the file!" << endl;
//         return;
//     }

//     /* Convert metadata into a displayable format and show it */
//     map<string, string> metadata = Metadata::convertTagToMap(currentTag, currentFileRef.audioProperties());
//     MetadataView metadataView;
//     metadataView.displayMetadata(metadata);
// }

// /* Handles various metadata editing actions */
// void MetadataController::handleAction(int action) {
//     if (!currentTag) {
//         cerr << "Error: No metadata loaded to edit!" << endl;

//         /* Switch to Media File View if no metadata is loaded */
//         MediaFileController* mediaFileController = dynamic_cast<MediaFileController*>(
//                 ManagerController::getInstance().getController("MediaFile"));

//             if (!mediaFileController) {
//                 cerr << "Error: MediaFileController is not available!" << endl;
//             }
//             ManagerController::getInstance().getManagerView()->setView("MediaFile");
//             cout << "\nSwitching to Media File View..." << endl;
//             mediaFileController->scanAndDisplayMedia();
//         return;
//     }

//     string newValue;
//     int newYear;

//     /* Handle metadata editing actions */
//     switch (action) {
//         case ACTION_EDIT_TITLE: {
//             /* Edit the title of the media file */
//             handleEditAction("Title", "Enter new title...", [&](const string& value) {
//                 currentTag->setTitle(TagLib::String(value));
//             });
//             break;
//         }

//         case ACTION_EDIT_ARTIST: {
//             /* Edit the artist of the media file */
//             handleEditAction("Artist", "Enter new artist...", [&](const string& value) {
//                 currentTag->setArtist(TagLib::String(value));
//             });
//             break;
//         }

//         case ACTION_EDIT_ALBUM: {
//             /* Edit the album of the media file */
//             handleEditAction("Album", "Enter new album...", [&](const string& value) {
//             currentTag->setAlbum(TagLib::String(value));
//             });
//             break;
//         }

//         case ACTION_EDIT_GENRE: {
//             /* Edit the genre of the media file */
//             handleEditAction("Genre", "Enter new genre...", [&](const string& value) {
//                 currentTag->setGenre(TagLib::String(value));
//             });
//             break;
//         }

//         case ACTION_EDIT_YEAR: {
//             /* Edit the year of the media file */
//             handleEditAction("Year", "Enter new year...", [&](const string& value) {
//                 int year = stoi(value);
//                 currentTag->setYear(year);
//             });
//             break;
//         }

//         case ACTION_EXIT_METADATA_EDITING: {
//             /* Save metadata and return to Media File View */
//             MediaFileController* mediaFileController = dynamic_cast<MediaFileController*>(
//                 ManagerController::getInstance().getController("MediaFile"));

//             if (!mediaFileController) {
//                 cerr << "Error: MediaFileController is not available!" << endl;
//                 break;
//             }
//             ManagerController::getInstance().getManagerView()->setView("MediaFile");
//             clearTerminal();
//             mediaFileController->scanAndDisplayMedia();
//             return;
//         }
//         default:
//             cerr << "Invalid choice! Please try again." << endl;
//             break;
//     }

//     /* Reload metadata after editing */
//     currentFileRef = TagLib::FileRef(currentFilePath.c_str());
//     if (currentFileRef.isNull()) {
//         cerr << "Error: Unable to refresh metadata!" << endl;
//         return;
//     }

//     handleShowMetadata(currentFilePath);
// }

// /* Saves the current metadata back to the file */
// void MetadataController::saveMetadata() {
//     if (currentFileRef.isNull()) {
//         cerr << "Error: currentFileRef is null! Cannot save metadata." << endl;
//         return;
//     }

//     /* Save metadata and handle success or error messages */
//     if (currentFileRef.save()) {
//         cout << "Metadata saved successfully!" << endl;
//     } else {
//         cerr << "Error: Could not save metadata to file." << endl;
//     }
// }

// /* Handles editing a metadata field */
// void MetadataController::handleEditAction(const string& field_name, const string& placeholder, function<void(const string&)> updateField) {
//     string new_value;
//     string result_message;
//     Decorator message_style;

//     /* Display an input box for the user to enter the new value */
//     auto input_box = Input(&new_value, placeholder);
//     auto input_renderer = Renderer(input_box, [&] {
//         return vbox({
//             text("Edit " + field_name) | bold | center,
//             separator(),
//             text("Enter new " + field_name + ":") | center,
//             input_box->Render() | border,
//             separator(),
//             text("Press ENTER to confirm, ESC to cancel.") | dim | center,
//         }) | center;
//     });

//     auto screen = ScreenInteractive::TerminalOutput();
//     bool confirmed = false;

//     /* Catch user input to confirm or cancel the edit */
//     auto main_component = CatchEvent(input_renderer, [&](Event event) {
//         if (event == Event::Return) {
//             confirmed = true;
//             screen.ExitLoopClosure()();
//             return true;
//         }
//         if (event == Event::Escape) {
//             confirmed = false;
//             screen.ExitLoopClosure()();
//             return true;
//         }
//         return false;
//     });

//     screen.Loop(main_component);

//     /* Display the result of the edit action */
//     if (confirmed && !new_value.empty()) {
//         updateField(new_value);
//         saveMetadata();
//         result_message = field_name + " updated successfully.";
//         message_style = color(Color::Green);
//     } else if (!confirmed) {
//         result_message = field_name + " update cancelled.";
//         message_style = color(Color::Yellow);
//     } else {
//         result_message = "Error: " + field_name + " cannot be empty.";
//         message_style = color(Color::Red);
//     }

//     /* Display the result message to the user */
//     auto result_renderer = Renderer([&] {
//         return vbox({
//             text(result_message) | bold | message_style | center,
//             separator(),
//             text("Press ENTER to continue...") | dim | center,
//         }) | center;
//     });

//     auto result_component = CatchEvent(result_renderer, [&](Event event) {
//         if (event == Event::Return) {
//             screen.ExitLoopClosure()();
//             return true;
//         }
//         return false;
//     });

//     screen.Loop(result_component);
//     clearTerminal();
// }

MetadataController::MetadataController(){ managerController = &ManagerController::getInstance();}
#include "controller/MetadataController.h"
#include "common/Enum.h"
#include "common/TerminalUtils.h"
#include <iostream>
#include <climits>

/* Sets the current tag object */
void MetadataController::setCurrentTag(TagLib::Tag* tag) {
    currentTag = tag;
}

/* Handles displaying metadata of a file */
void MetadataController::handleShowMetadata(const std::string& filepath) {
    if (filepath.empty()) {
        std::cerr << "Error: Invalid file path!" << std::endl;
        return;
    }

    /* Load the file and its metadata */
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

    /* Convert metadata into a displayable format and show it */
    std::map<std::string, std::string> metadata = Metadata::convertTagToMap(currentTag, currentFileRef.audioProperties());
    // MetadataView metadataView;
    // metadataView.displayMetadata(metadata);
    metadataView->displayMetadata(metadata);
}

/* Handles various metadata editing actions */
void MetadataController::handleAction(int action) {
    if (!currentTag) {
        std::cerr << "Error: No metadata loaded to edit!" << std::endl;

        /* Switch to Media File View if no metadata is loaded */
        MediaFileController* mediaFileController = dynamic_cast<MediaFileController*>(
            managerController->getController("MediaFile"));

        if (!mediaFileController) {
            std::cerr << "Error: MediaFileController is not available!" << std::endl;
        } else {
            managerController->getManagerView()->setView("MediaFile");
            std::cout << "\nSwitching to Media File View..." << std::endl;
            mediaFileController->scanAndDisplayMedia();
        }
        return;
    }

    std::string newValue;
    int newYear;

    /* Handle metadata editing actions */
    switch (action) {
        case ACTION_EDIT_TITLE: {
            /* Edit the title of the media file */
            handleEditAction("Title", "Enter new title...", [&](const std::string& value) {
                currentTag->setTitle(TagLib::String(value));
            });
            break;
        }

        case ACTION_EDIT_ARTIST: {
            /* Edit the artist of the media file */
            handleEditAction("Artist", "Enter new artist...", [&](const std::string& value) {
                currentTag->setArtist(TagLib::String(value));
            });
            break;
        }

        case ACTION_EDIT_ALBUM: {
            /* Edit the album of the media file */
            handleEditAction("Album", "Enter new album...", [&](const std::string& value) {
                currentTag->setAlbum(TagLib::String(value));
            });
            break;
        }

        case ACTION_EDIT_GENRE: {
            /* Edit the genre of the media file */
            handleEditAction("Genre", "Enter new genre...", [&](const std::string& value) {
                currentTag->setGenre(TagLib::String(value));
            });
            break;
        }

        case ACTION_EDIT_YEAR: {
            /* Edit the year of the media file */
            handleEditAction("Year", "Enter new year...", [&](const std::string& value) {
                int year = std::stoi(value);
                currentTag->setYear(year);
            });
            break;
        }

        case ACTION_EXIT_METADATA_EDITING: {
            /* Save metadata and return to Media File View */
            MediaFileController* mediaFileController = dynamic_cast<MediaFileController*>(
                managerController->getController("MediaFile"));

            if (!mediaFileController) {
                std::cerr << "Error: MediaFileController is not available!" << std::endl;
                break;
            }
            managerController->getManagerView()->setView("MediaFile");
            clearTerminal();
            mediaFileController->scanAndDisplayMedia();
            return;
        }
        default:
            std::cerr << "Invalid choice! Please try again." << std::endl;
            break;
    }

    /* Reload metadata after editing */
    currentFileRef = TagLib::FileRef(currentFilePath.c_str());
    if (currentFileRef.isNull()) {
        std::cerr << "Error: Unable to refresh metadata!" << std::endl;
        return;
    }

    handleShowMetadata(currentFilePath);
}

/* Saves the current metadata back to the file */
void MetadataController::saveMetadata() {
    if (currentFileRef.isNull()) {
        std::cerr << "Error: currentFileRef is null! Cannot save metadata." << std::endl;
        return;
    }

    /* Save metadata and handle success or error messages */
    if (currentFileRef.save()) {
        std::cout << "Metadata saved successfully!" << std::endl;
    } else {
        std::cerr << "Error: Could not save metadata to file." << std::endl;
    }
}

/* Handles editing a metadata field */
void MetadataController::handleEditAction(
    const std::string& field_name, 
    const std::string& placeholder, 
    std::function<void(const std::string&)> updateField) {

    std::string new_value;
    std::string result_message;
    Decorator message_style;

    /* Display an input box for the user to enter the new value */
    auto input_box = Input(&new_value, placeholder);
    auto input_renderer = Renderer(input_box, [&] {
        return vbox({
            text("Edit " + field_name) | bold | center,
            separator(),
            text("Enter new " + field_name + ":") | center,
            input_box->Render() | border,
            separator(),
            text("Press ENTER to confirm, ESC to cancel.") | dim | center,
        }) | center;
    });

    auto screen = ScreenInteractive::TerminalOutput();
    bool confirmed = false;

    /* Catch user input to confirm or cancel the edit */
    auto main_component = CatchEvent(input_renderer, [&](Event event) {
        if (event == Event::Return) {
            confirmed = true;
            screen.ExitLoopClosure()();
            return true;
        }
        if (event == Event::Escape) {
            confirmed = false;
            screen.ExitLoopClosure()();
            return true;
        }
        return false;
    });

    screen.Loop(main_component);

    /* Display the result of the edit action */
    if (confirmed && !new_value.empty()) {
        updateField(new_value);
        saveMetadata();
        result_message = field_name + " updated successfully.";
        message_style = color(Color::Green);
    } else if (!confirmed) {
        result_message = field_name + " update cancelled.";
        message_style = color(Color::Yellow);
    } else {
        result_message = "Error: " + field_name + " cannot be empty.";
        message_style = color(Color::Red);
    }

    /* Display the result message to the user */
    auto result_renderer = Renderer([&] {
        return vbox({
            text(result_message) | bold | message_style | center,
            separator(),
            text("Press ENTER to continue...") | dim | center,
        }) | center;
    });

    auto result_component = CatchEvent(result_renderer, [&](Event event) {
        if (event == Event::Return) {
            screen.ExitLoopClosure()();
            return true;
        }
        return false;
    });

    screen.Loop(result_component);
    clearTerminal();
}
