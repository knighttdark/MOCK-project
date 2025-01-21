#include "controller/MetadataController.h"
#include "common/Enum.h"
#include "common/TerminalUtils.h"
#include <iostream>
#include <climits>


void MetadataController::setCurrentTag(TagLib::Tag* tag) {
    currentTag = tag;
}


void MetadataController::handleShowMetadata(const string& filepath) {
    if (filepath.empty()) {
        cerr << "Error: Invalid file path!" << endl;
        return;
    }

    currentFilePath = filepath;
    currentFileRef = TagLib::FileRef(filepath.c_str());
    if (currentFileRef.isNull()) {
        cerr << "Error: Unable to open file. Please check the file path!" << endl;
        return;
    }

    currentTag = currentFileRef.tag();
    if (!currentTag) {
        cerr << "Error: No metadata available in the file!" << endl;
        return;
    }

    if (!currentFileRef.audioProperties()) {
        cerr << "Error: No audio properties available for the file!" << endl;
        return;
    }

    map<string, string> metadata = Metadata::convertTagToMap(currentTag, currentFileRef.audioProperties());
    MetadataView metadataView;
    metadataView.displayMetadata(metadata);
}


void MetadataController::handleAction(int action) {
    if (!currentTag) {
        cerr << "Error: No metadata loaded to edit!" << endl;
        MediaFileController* mediaFileController = dynamic_cast<MediaFileController*>(
                ManagerController::getInstance().getController("MediaFile"));

            if (!mediaFileController) {
                cerr << "Error: MediaFileController is not available!" << endl;
            }
            ManagerController::getInstance().getManagerView()->setView("MediaFile");
            cout << "\nSwitching to Media File View..." << endl;
            mediaFileController->scanAndDisplayMedia();
        return;
    }

    string newValue;
    int newYear;

    switch (action) {
        case ACTION_EDIT_TITLE: {
            handleEditAction("Title", "Enter new title...", [&](const std::string& value) {
                currentTag->setTitle(TagLib::String(value));
            });
            break;
        }

        case ACTION_EDIT_ARTIST: {
            handleEditAction("Artist", "Enter new artist...", [&](const std::string& value) {
                currentTag->setArtist(TagLib::String(value));
            });
            break;
        }

        case ACTION_EDIT_ALBUM: {
            handleEditAction("Album", "Enter new album...", [&](const std::string& value) {
            currentTag->setAlbum(TagLib::String(value));
            });
            break;
        }

        case ACTION_EDIT_GENRE: {
            handleEditAction("Genre", "Enter new genre...", [&](const std::string& value) {
                currentTag->setGenre(TagLib::String(value));
            });
            break;
        }

        case ACTION_EDIT_YEAR: {
            handleEditAction("Year", "Enter new year...", [&](const std::string& value) {
                int year = std::stoi(value);
                currentTag->setYear(year);
            });
            break;
        }


        case ACTION_EXIT_METADATA_EDITING: {
            MediaFileController* mediaFileController = dynamic_cast<MediaFileController*>(
                ManagerController::getInstance().getController("MediaFile"));

            if (!mediaFileController) {
                cerr << "Error: MediaFileController is not available!" << endl;
                break;
            }
            ManagerController::getInstance().getManagerView()->setView("MediaFile");
            clearTerminal();
            mediaFileController->scanAndDisplayMedia();
            return;
        }
        default:
            cerr << "Invalid choice! Please try again." << endl;
            break;
    }

    currentFileRef = TagLib::FileRef(currentFilePath.c_str());
    if (currentFileRef.isNull()) {
        cerr << "Error: Unable to refresh metadata!" << endl;
        return;
    }

    handleShowMetadata(currentFilePath);
}


void MetadataController::saveMetadata() {
    if (currentFileRef.isNull()) {
        cerr << "Error: currentFileRef is null! Cannot save metadata." << endl;
        return;
    }

    if (currentFileRef.save()) {
        cout << "Metadata saved successfully!" << endl;
    } else {
        cerr << "Error: Could not save metadata to file." << endl;
    }
}

void MetadataController::handleEditAction(const std::string& field_name, const std::string& placeholder, std::function<void(const std::string&)> updateField) {
    std::string new_value;
    std::string result_message;
    Decorator message_style;

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