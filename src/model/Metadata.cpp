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
map<string, string> Metadata::convertTagToMap(TagLib::Tag* tag, TagLib::AudioProperties* audioProperties) {
    map<string, string> metadata;

    // Metadata từ TagLib::Tag*
    if (tag) {
        metadata["Title"] = tag->title().isEmpty() ? "N/A" : tag->title().toCString(true);
        metadata["Artist"] = tag->artist().isEmpty() ? "N/A" : tag->artist().toCString(true);
        metadata["Album"] = tag->album().isEmpty() ? "N/A" : tag->album().toCString(true);
        metadata["Year"] = tag->year() == 0 ? "N/A" : to_string(tag->year());
        metadata["Comment"] = tag->comment().isEmpty() ? "N/A" : tag->comment().toCString(true);
        metadata["Genre"] = tag->genre().isEmpty() ? "N/A" : tag->genre().toCString(true);
    }

    // Properties từ TagLib::AudioProperties*
    if (audioProperties) {
        metadata["Duration"] = to_string(audioProperties->length()) + " seconds";
        metadata["Bitrate"] = to_string(audioProperties->bitrate()) + " kbps";
        metadata["Sample Rate"] = to_string(audioProperties->sampleRate()) + " Hz";
        metadata["Channels"] = to_string(audioProperties->channels());
    } else {
        metadata["Duration"] = "N/A";
        metadata["Bitrate"] = "N/A";
        metadata["Sample Rate"] = "N/A";
        metadata["Channels"] = "N/A";
    }

    return metadata;
}

