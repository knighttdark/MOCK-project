#ifndef METADATA_H
#define METADATA_H

#include <string>
#include <unordered_map>
#include <map>
#include <taglib/tag.h>
#include <taglib/audioproperties.h>

using namespace std;

/* Manage metadata operations */
class Metadata {
private:
    unordered_map<string, string> metadata;
public:
    Metadata() = default;
    unordered_map<string, string> getMetadata() const;
    void setData(const unordered_map<string, string>& data);
    string getValue(const string& key) const;
    void setValue(const string& key, const string& value);
    static map<string, string> convertTagToMap(TagLib::Tag* tag, TagLib::AudioProperties* audioProperties);
};

#endif
