
#ifndef METADATA_HPP
#define METADATA_HPP

#include <string>
#include <unordered_map>

using namespace std;

class Metadata {
private:
    unordered_map<string, string> metadata;

public:
    // Constructors
    Metadata() = default;

    // Getters and Setters
    unordered_map<string, string> getMetadata() const;
    void setData(const unordered_map<string, string>& data);
    string getValue(const string& key) const;
    void setValue(const string& key, const string& value);
};

#endif // METADATA_HPP


// #ifndef METADATA_H
// #define METADATA_H

// #include <map>
// #include <string>

// class Metadata {
// private:
//     std::map<std::string, std::string> metadata;

// public:
//     std::map<std::string, std::string> getMetaData() const;
//     void setData(const std::map<std::string, std::string>& data);
//     std::string getValue(const std::string& key) const;
//     void setValue(const std::string& key, const std::string& value);
//     void displayMetadata() const;
// };

// #endif // METADATA_H
