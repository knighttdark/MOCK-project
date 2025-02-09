// #ifndef PLAYLISTCONTROLLER_H
// #define PLAYLISTCONTROLLER_H

// #include <string>
// #include "common/BaseController.h"

// /* Manage playlist actions */
// class PlaylistController : public BaseController {
// public:
//     PlaylistController();

//     void handleAction(int action) override;
//     virtual void createPlaylist(const string& name);
//     virtual void deletePlaylist();
//     virtual void viewPlaylistDetails(const string& name);
//     virtual void listAllPlaylists();
//     void playPlaylist(const string& name);
// };

// #endif
#ifndef PLAYLISTCONTROLLER_H
#define PLAYLISTCONTROLLER_H

// Forward declaration of ManagerController to avoid circular dependency
class ManagerController;
#include "controller/ManagerController.h"
#include <string>
#include "common/BaseController.h"
#include "view/ManagerView.h"
/* Manage playlist actions */
class PlaylistController : public BaseController {
private:
    ManagerController* managerController;
      

public:
    PlaylistController();  // Default constructor
    explicit PlaylistController(ManagerController* controller)
        : managerController(controller ? controller : &ManagerController::getInstance()) {}

    void handleAction(int action) override;
    virtual void createPlaylist(const string& name);
    virtual void deletePlaylist();
    virtual void viewPlaylistDetails(const string& name);
    virtual void listAllPlaylists();
    virtual void playPlaylist(const string& name);
};

#endif
