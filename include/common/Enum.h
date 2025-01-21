#ifndef ENUM_H
#define ENUM_H

/* Enum for DefaultScreenController */
enum DefaultScreenAction {
    ACTION_SWITCH_TO_MEDIA_FILE = 1,
    ACTION_SWITCH_TO_PLAYLIST = 2,
    ACTION_EXIT_APPLICATION = 0
};

/* Enum for MediaFileController Scan Options */
enum MediaFileScanOption {
    RETURN_HOME = 0,
    SCAN_DIRECTORY = 1,
    SCAN_USB = 2
};

/* Enum for MediaFileController */
enum MediaFileAction {
    ACTION_SHOW_PROPERTIES = 1,
    ACTION_PLAY_MEDIA = 2,
    ACTION_ADD_TO_PLAYLIST = 3,
    ACTION_RETURN_TO_PLAYING = 4,
    ACTION_NEXT_PAGE = 5,
    ACTION_PREVIOUS_PAGE = 6,
    ACTION_RETURN_HOME = 0
};

/* Enum for MetadataController */
enum MetadataAction {
    ACTION_EDIT_TITLE = 1,
    ACTION_EDIT_ARTIST = 2,
    ACTION_EDIT_ALBUM = 3,
    ACTION_EDIT_GENRE = 4,
    ACTION_EDIT_YEAR = 5,
    ACTION_EXIT_METADATA_EDITING = 0
};

/* Enum for PlaylistController */
enum PlaylistAction {
    ACTION_CREATE_PLAYLIST = 1,
    ACTION_DELETE_PLAYLIST = 2,
    ACTION_VIEW_PLAYLIST_DETAILS = 3,
    ACTION_LIST_ALL_PLAYLISTS = 4,
    ACTION_PLAY_PLAYLISTS = 5,
    ACTION_EXIT_PLAYLIST_MENU = 0
};

/* Enum for PlayingMediaController */
enum PlayingMediaAction {
    ACTION_PLAY_PAUSE = 1,
    ACTION_SKIP_NEXT = 2,
    ACTION_SKIP_PREVIOUS = 3,
    ACTION_ADJUST_VOLUME = 4,
    ACTION_STOP = 5,
    ACTION_EXIT_PLAYING_MENU = 0
};

#endif /* ENUM_H */