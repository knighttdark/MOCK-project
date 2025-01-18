#ifndef EXCEPTION_H
#define EXCEPTION_H

#include <string>
#include <iostream>
#include <stdbool.h>
#include <stdexcept>
#include <functional>
#include <regex>
#include <climits>
#include <fstream>

using namespace std;

class Exception {
public:
    /* Common validations (throw exception if invalid) */
    static void checkStringNotEmpty(string value, string fieldName);
    static void checkStringMaxLength(string value, size_t maxLength, string fieldName);
    static void checkIntRange(int value, int min, int max, string fieldName);

    static bool validateInputDefaultScreen(int choice);
    static bool validateInputMediaFileView(int choice);
    static bool validateInputOptionScan(int choice);
    static bool validateInputMetadataView(int choice);
    static bool validateInputPlaylistView(int choice);

    /* Specific validations */
    static bool checkInputValidType();
    static string getValidatedInput_String(string prompt, function<bool(string)> validator);
    static int getValidatedInput_Int(string prompt, function<bool(int)> validator);


    static void checkInputFilePath(const string& filePath);
    static string getValidatedInput_FilePath(string prompt);
};

#endif /* EXCEPTION_H */