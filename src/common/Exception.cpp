#include "common/Exception.h"
#include <iostream>

/* Check if a string is not empty */
void Exception::checkStringNotEmpty(string value, string fieldName) {
    if (value.empty()) {
        throw invalid_argument(fieldName + " cannot be empty.");
    }
}

/* Check if a string exceeds the maximum length */
void Exception::checkStringMaxLength(string value, size_t maxLength, string fieldName) {
    if (value.length() > maxLength) {
        throw invalid_argument(fieldName + " exceeds maximum length of " + to_string(maxLength) + " characters.");
    }
}

/* Check if an integer is within a specified range */
void Exception::checkIntRange(int value, int min, int max, string fieldName) {
    if (value < min || value > max) {
        throw invalid_argument(fieldName + " must be between " + to_string(min) + " and " + to_string(max) + ".");
    }
}

/* Validate input for the default screen menu (choice must be between 0 and 2) */
bool Exception::validateInputDefaultScreen(int choice) {
    checkIntRange(choice, 0, 2, "choice");
    return true;
}

/* Validate input for the Media File View menu (choice must be between 0 and 5) */
bool Exception::validateInputMediaFileView(int choice) {
    checkIntRange(choice, 0, 5, "choice");
    return true;
}

/* Validate input for scanning options (choice must be between 0 and 2) */
bool Exception::validateInputOptionScan(int choice) {
    checkIntRange(choice, 0, 2, "choice");
    return true;
}

/* Validate input for the Metadata View menu (choice must be between 0 and 6) */
bool Exception::validateInputMetadataView(int choice) {
    checkIntRange(choice, 0, 6, "choice");
    return true;
}

/* Validate input for the Playlist View menu (choice must be between 0 and 5) */
bool Exception::validateInputPlaylistView(int choice) {
    checkIntRange(choice, 0, 5, "choice");
    return true;
}

/* Validate input for the Playing View menu (choice must be between 0 and 5) */
bool Exception::validateInputPlayingView(int choice) {
    checkIntRange(choice, 0, 5, "choice");
    return true;
}

/* Check if a file path is valid and the file exists */
void Exception::checkInputFilePath(const string& filePath) {
    ifstream file(filePath);
    if (!file.is_open()) {
        throw invalid_argument("Error: File path is invalid or file does not exist.");
    }
    file.close();
}

/* Check if the user input is a valid type (numeric, not failed input) */
bool Exception::checkInputValidType() {
    if (cin.fail()) {
        cin.clear();
        cin.ignore(INT_MAX, '\n');
        cout << "Invalid input. Please enter a valid number.\n";
        return false;
    }
    return true;
}

/* Get a validated string input from the user */
string Exception::getValidatedInput_String(string prompt, function<bool(string)> validator) {
    string input;
    while (true) {
        try {
            cout << prompt;
            getline(cin, input);
            validator(input);
            return input;
        } catch (const invalid_argument& e) {
            cerr << "Error: " << e.what() << endl;
        }
    }
}

/* Get a validated integer input from the user */
int Exception::getValidatedInput_Int(string prompt, function<bool(int)> validator) {
    int input;
    while (true) {
        try {
            cout << prompt;
            cin >> input;
            if (!checkInputValidType()) continue;
            validator(input);
            cin.ignore(INT_MAX, '\n');
            return input;
        } catch (const invalid_argument& e) {
            cerr << "Error: " << e.what() << endl;
            cin.clear();
            cin.ignore(INT_MAX, '\n');
        }
    }
}

/* Get a validated file path input from the user */
string Exception::getValidatedInput_FilePath(string prompt) {
    string filePath;
    while (true) {
        try {
            cout << prompt;
            getline(cin, filePath);
            checkInputFilePath(filePath);
            return filePath;
        } catch (const invalid_argument& e) {
            cerr << "Error: " << e.what() << endl;
            cout << "Please try again.\n";
        }
    }
}
