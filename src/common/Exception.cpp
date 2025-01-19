#include "common/Exception.h"
#include <iostream>

using namespace std;

// Common validations (throw exception if invalid)
void Exception::checkStringNotEmpty(string value, string fieldName) {
    if (value.empty()) {
        throw invalid_argument(fieldName + " cannot be empty.");
    }
}

void Exception::checkStringMaxLength(string value, size_t maxLength, string fieldName) {
    if (value.length() > maxLength) {
        throw invalid_argument(fieldName + " exceeds maximum length of " + to_string(maxLength) + " characters.");
    }
}

void Exception::checkIntRange(int value, int min, int max, string fieldName) {
    if (value < min || value > max) {
        throw invalid_argument(fieldName + " must be between " + to_string(min) + " and " + to_string(max) + ".");
    }
}
//// Specific validations


bool Exception::validateInputDefaultScreen(int choice)
{
    checkIntRange(choice, 0, 2, "choice");
    return true;
}

bool Exception::validateInputMediaFileView(int choice)
{
    checkIntRange(choice, 0, 5, "choice");
    return true;
}

bool Exception::validateInputOptionScan(int choice)
{
    checkIntRange(choice, 0, 2, "choice");
    return true;
}

bool Exception::validateInputMetadataView(int choice)
{
    checkIntRange(choice, 0, 6, "choice");
    return true;
}

bool Exception::validateInputPlaylistView(int choice)
{
    checkIntRange(choice, 0, 5, "choice");
    return true;
}

bool Exception::validateInputPlayingView(int choice)
{
    checkIntRange(choice, 0, 4, "choice");
    return true;
}


void Exception::checkInputFilePath(const string& filePath) {
    ifstream file(filePath);
    if (!file.is_open()) {
        throw invalid_argument("Error: File path is invalid or file does not exist.");
    }
    file.close();
}


bool Exception::checkInputValidType() {
    if (cin.fail()) {
        cin.clear();
        cin.ignore(INT_MAX, '\n');
        cout << "Invalid input. Please enter a valid number.\n";
        return false;
    }
    return true;
}

// General method to get input with validation
string Exception::getValidatedInput_String(string prompt, function<bool(string)> validator) {
    string input;
    while (true) {
        try {
            cout << prompt; //
            getline(cin, input);  // Read input from user
            validator(input);     // Validate input using the provided validator function
            return input;         // Return if valid
        } catch (const invalid_argument& e) {
            cerr << "Error: " << e.what() << endl;
        }
    }
}
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



string Exception::getValidatedInput_FilePath(string prompt) {
    string filePath;
    while (true) {
        try {
            cout << prompt;
            getline(cin, filePath);
            checkInputFilePath(filePath); // Kiểm tra tính hợp lệ của đường dẫn
            return filePath;
        } catch (const invalid_argument& e) {
            cerr << "Error: " << e.what() << endl;
            cout << "Please try again.\n";
        }
    }
}
