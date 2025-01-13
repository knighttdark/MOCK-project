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
    unordered_map<string, string> metadata; /* Metadata map */

public:
    Metadata() = default; /* Default constructor */

    unordered_map<string, string> getMetadata() const; /* Get metadata */
    void setData(const unordered_map<string, string>& data); /* Set metadata */
    string getValue(const string& key) const; /* Get value by key */
    void setValue(const string& key, const string& value); /* Set value by key */
    static map<string, string> convertTagToMap(TagLib::Tag* tag, TagLib::AudioProperties* audioProperties); /* Convert Tag to map */
};

#endif /* METADATA_H */