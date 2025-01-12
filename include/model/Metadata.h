
#ifndef METADATA_HPP
#define METADATA_HPP

#include <string>
#include <unordered_map>
#include <map>
#include <taglib/tag.h>
#include <taglib/audioproperties.h>

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
    static std::map<std::string, std::string> convertTagToMap(TagLib::Tag* tag, TagLib::AudioProperties* audioProperties);
};

#endif // METADATA_HPP

