// #ifndef MEDIAFILE_H
// #define MEDIAFILE_H

// #include <string>
// #include "Metadata.h"

// using namespace std;

// class MediaFile {
// private:
//     string name;
//     string path;
//     string type;
//     Metadata metadata;

// public:
//     // Constructors
//     MediaFile() = default;
//     MediaFile(const string& name, const string& path, const string& type);

//     // Getters and Setters
//     string getName() const;
//     string getPath() const;
//     string getType() const;
//     Metadata getMetadata() const;
//     void setMetadata(const Metadata& metadata);

//     // Overload toán tử ==
//     bool operator==(const MediaFile& other) const;
//     string getInfo() const;

// };

// #endif // MEDIAFILE_HPP
// MediaFile.h
#ifndef MEDIAFILE_H
#define MEDIAFILE_H

#include <string>
#include "Metadata.h"

class MediaFile {
private:
    int index; // Chỉ số của MediaFile trong danh sách
    string name;
    string path;
    string type;
    Metadata metadata;

public:
    // Constructors
    MediaFile();
    MediaFile(int index, const string& name, const string& path, const string& type);

    // Getters and Setters
    int getIndex() const;
    void setIndex(int newIndex);

    string getName() const;
    string getPath() const;
    string getType() const;
    Metadata getMetadata() const;
    void setMetadata(const Metadata& metadata);

    string getInfo() const;
};

#endif // MEDIAFILE_H