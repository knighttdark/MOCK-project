#ifndef MOCK_CLASS_H
#define MOCK_CLASS_H

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "BaseView.h"
#include "BaseController.h"
#include "controller/ManagerController.h"
#include "view/ManagerView.h"
#include "controller/MediaFileController.h"
#include "controller/PlaylistController.h"
#include "controller/PlayingMediaController.h"
#include "model/MediaLibrary.h"
#include "model/PlaylistLibrary.h"
#include "model/MediaFile.h"
#include "model/Playlist.h"
#include "model/ManagerModel.h"
#include "controller/MetadataController.h"
#include "view/PlayingView.h"
#include "view/MediaFileView.h"
#include "view/MetadataView.h"

using ::testing::Return;
using ::testing::Invoke;

// Mock cho BaseView
class MockView : public BaseView {
public:
    MOCK_METHOD(int, showMenu, (), (override));
};

// Mock cho BaseController
class MockController : public BaseController {
public:
    MOCK_METHOD(void, handleAction, (int action), (override));
};

// Mock cho ManagerController
class MockManagerController : public ManagerController {
public:
    virtual ~MockManagerController() = default;
    MOCK_METHOD(BaseController*, getController, (const std::string& key), (const, override));
    MOCK_METHOD(ManagerView*, getManagerView, (), (override));
    MOCK_METHOD(ManagerModel*, getManagerModel, (), (override));

};

// Mock cho ManagerView
class MockManagerView : public ManagerView {
public:
    virtual ~MockManagerView() = default;
    MOCK_METHOD(void, setView, (const std::string& key), (override));
    MOCK_METHOD(BaseView*, getView, (), (const, override));
    MOCK_METHOD(std::string, getCurrentViewKey, (), (const, override));
};

// Mock cho MediaFileController
class MockMediaFileController : public MediaFileController {
public:
    MOCK_METHOD(void, handleActionScan, (int option), (override));
    MOCK_METHOD(void, scanAndDisplayMedia, (), (override));
};

class MockPlayingView : public PlayingView {
public:
    MOCK_METHOD(int, showMenu, (), (override));  // Mock relevant methods
};

// Mock cho MediaFileView
class MockMediaFileView : public MediaFileView {
public:
    MOCK_METHOD(int, showMenu, (), (override));
    MOCK_METHOD(int, showOptionScan, (), (override));
    MOCK_METHOD(int, getSelectedMediaID, (), (const, override));
    MOCK_METHOD(void, displayMediaFiles, (const std::vector<std::string>& files, int page, const std::string& notification), (override));
    MOCK_METHOD(std::string, promptDirectoryInput, (), (override));
};

// Mock cho PlaylistController
class MockPlaylistController : public PlaylistController {
public:
    MOCK_METHOD(void, listAllPlaylists, (), (override));
    MOCK_METHOD(void, createPlaylist, (const std::string& name), (override));
    MOCK_METHOD(void, deletePlaylist, (), (override));
    MOCK_METHOD(void, viewPlaylistDetails, (const std::string& name), (override));
    MOCK_METHOD(void, playPlaylist, (const std::string& name), (override));

};

// Mock cho PlaylistView
class MockPlaylistView : public PlaylistView {
public:
    MOCK_METHOD(void, displayPlaylists, (const std::vector<Playlist>& playlists), (override));
    MOCK_METHOD(int, getSelectedPlaylistID, (), (const, override));
    MOCK_METHOD(void, displayPlaylistDetails, (const Playlist&), (override));
};

// Mock cho PlayingMediaController
class MockPlayingMediaController : public PlayingMediaController {
public:
    virtual ~MockPlayingMediaController() = default;
    MOCK_METHOD(void, playMediaFile, (MediaFile* mediaFile), (override));
    MOCK_METHOD(MediaFile*, getCurrentMediaFile, (), (const, override));
    MOCK_METHOD(bool, isCurrentlyPlaying, (), (const, override));
    MOCK_METHOD(void, stop, (), (override));
    MOCK_METHOD(void, handleAction, (int action), (override));
    MOCK_METHOD(void, skipToNext, (), (override));
    MOCK_METHOD(void, skipToPrevious, (), (override));
    MOCK_METHOD(void, adjustVolume, (int level), (override));
    MOCK_METHOD(void, playVideo, (const std::string& videoPath), (override));
    MOCK_METHOD(void, playPlaylist, (std::vector<MediaFile>&), (override));

};

// Mock cho MediaLibrary
class MockMediaLibrary : public MediaLibrary {
public:
    virtual ~MockMediaLibrary() = default;
    MOCK_METHOD(std::vector<MediaFile>&, getMediaFiles, (), (override));
    MOCK_METHOD(std::vector<MediaFile>, getMediaFilesForPage, (int page, int pageSize), (const, override));
    MOCK_METHOD(int, getTotalPages, (int pageSize), (const, override));
    MOCK_METHOD(void, scanDirectory, (const std::string& path), (override));
};

// Mock cho PlaylistLibrary
class MockPlaylistLibrary : public PlaylistLibrary {
public:
    MOCK_METHOD(std::vector<Playlist>&, getPlaylists, (), (override));
    MOCK_METHOD(void, saveToFile, (const std::string& filename), (const, override));
    MOCK_METHOD(Playlist*, getPlaylistByName, (const std::string&), (override));
    MOCK_METHOD(void, addPlaylist, (const Playlist&), (override));
    MOCK_METHOD(void, removePlaylist, (const std::string&), (override));
};

// Mock cho ManagerModel
class MockManagerModel : public ManagerModel {
public:
    virtual ~MockManagerModel() = default; 
    MOCK_METHOD(MediaLibrary&, getMediaLibrary, (), (override));
    MOCK_METHOD(PlaylistLibrary&, getPlaylistLibrary, (), (override));
    MOCK_METHOD(PlayingMediaModel&, getPlayingMedia, (), (override));
};

// Mock cho MediaFile
class MockMediaFile : public MediaFile {
public:
    MOCK_METHOD(int, getIndex, (), (const, override));
    MOCK_METHOD(std::string, getName, (), (const, override));
    MOCK_METHOD(std::string, getPath, (), (const, override));
};

// Mock cho Playlist
class MockPlaylist : public Playlist {
public:
    MOCK_METHOD(std::string, getName, (), (const, override));
    MOCK_METHOD(void, addSong, (const MediaFile& file), (override));
};

// Mock cho MetadataController
class MockMetadataController : public MetadataController {
public:
    MOCK_METHOD(void, handleShowMetadata, (const std::string& filepath), (override));
};

class MockFileSystem {
public:
    MOCK_METHOD(std::vector<std::string>, listUSBDevices, (const std::string&), ());
};

using MetadataMap = std::map<std::string, std::string>;

class MockMetadataView : public MetadataView {
public:
    MOCK_METHOD(void, displayMetadata, (const MetadataMap& metadata), (override));
    MOCK_METHOD(int, showMenu, (), (override));
};

#include <taglib/tag.h>

class MockTagLibTag : public TagLib::Tag {
public:
    MOCK_METHOD(TagLib::String, title, (), (const, override));
    MOCK_METHOD(TagLib::String, artist, (), (const, override));
    MOCK_METHOD(TagLib::String, album, (), (const, override));
    MOCK_METHOD(TagLib::String, comment, (), (const, override));
    MOCK_METHOD(TagLib::String, genre, (), (const, override));
    MOCK_METHOD(unsigned int, year, (), (const, override));
    MOCK_METHOD(unsigned int, track, (), (const, override));
    MOCK_METHOD(void, setTitle, (const TagLib::String &s), (override));
    MOCK_METHOD(void, setArtist, (const TagLib::String &s), (override));
    MOCK_METHOD(void, setAlbum, (const TagLib::String &s), (override));
    MOCK_METHOD(void, setComment, (const TagLib::String &s), (override));
    MOCK_METHOD(void, setGenre, (const TagLib::String &s), (override));
    MOCK_METHOD(void, setYear, (unsigned int i), (override));
    MOCK_METHOD(void, setTrack, (unsigned int i), (override));
};

class MockTagLibFileRef {
public:
    MOCK_METHOD(bool, isNull, (), (const));
    MOCK_METHOD(TagLib::Tag*, tag, (), (const));
    MOCK_METHOD(TagLib::AudioProperties*, audioProperties, (), (const));
};
#endif // MOCK_CLASS_H
