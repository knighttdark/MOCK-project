#include "common/TagLibUtils.h"
#include "common/Metadata.h"
#include <iostream>
#include <iomanip>

void printMetadata(TagLib::Tag* tag, TagLib::AudioProperties* audioProperties) {
    std::cout << "+---------------------+---------------------------+" << std::endl;
    std::cout << "|       Field         |          Value           |" << std::endl;
    std::cout << "+---------------------+---------------------------+" << std::endl;

    std::cout << "| " << std::setw(20) << std::left << "Title" 
              << "| " << std::setw(25) << tag->title().toCString() << " |" << std::endl;
    std::cout << "| " << std::setw(20) << std::left << "Artist" 
              << "| " << std::setw(25) << tag->artist().toCString() << " |" << std::endl;
    std::cout << "| " << std::setw(20) << std::left << "Album" 
              << "| " << std::setw(25) << tag->album().toCString() << " |" << std::endl;
    std::cout << "| " << std::setw(20) << std::left << "Year" 
              << "| " << std::setw(25) << tag->year() << " |" << std::endl;
    std::cout << "| " << std::setw(20) << std::left << "Duration (seconds)" 
              << "| " << std::setw(25) << audioProperties->length() << " |" << std::endl;
    std::cout << "+---------------------+---------------------------+" << std::endl;

    std::cout << "\nAudio Information:" << std::endl;
    std::cout << "+---------------------+---------------------------+" << std::endl;
    std::cout << "| " << std::setw(20) << std::left << "Channels" 
              << "| " << std::setw(25) << audioProperties->channels() << " |" << std::endl;
    std::cout << "| " << std::setw(20) << std::left << "Sample Rate (Hz)" 
              << "| " << std::setw(25) << audioProperties->sampleRate() << " |" << std::endl;
    std::cout << "| " << std::setw(20) << std::left << "Bitrate (kbps)" 
              << "| " << std::setw(25) << audioProperties->bitrate() << " |" << std::endl;
    std::cout << "+---------------------+---------------------------+" << std::endl;
}

void restoreMetadata(TagLib::Tag* tag, const Metadata& originalMetadata) {
    tag->setTitle(TagLib::String(originalMetadata.getValue("Title")));
    tag->setArtist(TagLib::String(originalMetadata.getValue("Artist")));
    tag->setAlbum(TagLib::String(originalMetadata.getValue("Album")));
    tag->setYear(std::stoi(originalMetadata.getValue("Year")));
    tag->setTrack(std::stoi(originalMetadata.getValue("Track")));
    tag->setGenre(TagLib::String(originalMetadata.getValue("Genre")));
}
