#include "model/Metadata.h"

unordered_map<string, string> Metadata::getMetadata() const {
    return metadata;
}

void Metadata::setData(const unordered_map<string, string>& data) {
    metadata = data;
}

string Metadata::getValue(const string& key) const {
    if (metadata.find(key) != metadata.end()) {
        return metadata.at(key);
    }
    return "";
}

void Metadata::setValue(const string& key, const string& value) {
    metadata[key] = value;
}



// #include "model/Metadata.h"
// #include <iostream>

// std::map<std::string, std::string> Metadata::getMetaData() const {
//     return metadata;
// }

// void Metadata::setData(const std::map<std::string, std::string>& data) {
//     metadata = data;
// }

// std::string Metadata::getValue(const std::string& key) const {
//     auto it = metadata.find(key);
//     if (it != metadata.end()) {
//         return it->second;
//     } else {
//         return "Key not found";
//     }
// }

// void Metadata::setValue(const std::string& key, const std::string& value) {
//     metadata[key] = value;
// }

// void Metadata::displayMetadata() const {
//     for (const auto& entry : metadata) {
//         std::cout << entry.first << ": " << entry.second << std::endl;
//     }
// }
