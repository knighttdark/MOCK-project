# CLI Media Browser and Player Application

## Overview
This project is a command-line interface (CLI) application for Linux that serves as a basic media (audio and video) browser and player with advanced features. It supports browsing media files, managing playlists, editing metadata, and playing music. Additionally, it integrates with the S32K144 board for external device control.

---

## Features

### **[Basic Features]**

#### **1. Application Behavior:**
1. The application can run:
   - From any directory on your computer.
   - From an external USB device (requires mounting the USB and parsing files).

#### **2. Available Options:**

1. **Media File Browsing:**
   - View a list of media files (audio and video) in the current folder and all subfolders.
   - Includes pagination: Display 25 files per page.

2. **Playlist Management:**
   - List existing playlists.
   - View a specific playlist.
   - Create, update, or delete playlists.

3. **Metadata Handling (using Taglib):**
   - Accessible from either the media list or a playlist.
   - Display metadata of a media file:
     - **Audio:** Track name, Album, Artist, Duration, Genre, Publisher, Publish Year, etc.
     - **Video:** Name, Size, Duration, Bitrate, Codec, etc.
   - Edit metadata:
     - Modify existing values.
     - Add new metadata keys.

4. **Music Playback (using SDL2 Library):**
   - Supports basic controls:
     - Play/Pause.
     - Next/Previous.
   - Automatically moves to the next song when the current song finishes.
   - Displays the current playback time and total duration.

5. **Device Volume Control:**
   - Change the volume of the device directly from the application.

---

### **[External Device Connection]**

#### **1. Integration with S32K144 Board:**
- Volume control via ADC on the board.
- Playback control using buttons:
  - Play, pause, stop, next, and previous music.
- Display the currently playing song’s information on the board’s screen.

#### **2. Deliverables:**

1. **Software Requirement Specification:**
   - Use case descriptions.
   - Detailed workflows.

2. **Design Document:**
   - **Architecture Design:**
     - Implementation of the MVC pattern.
   - **Detailed Design:**
     - Modules and component-level descriptions.

3. **Code & Unit Tests:**
   - Implementation of the core application.
   - Unit tests for individual components.

### Setup library UI

---

- [Github Mock-project](https://github.com/thanhetebkdn/MOCK-project)
- [Notion Mock-project](https://thanhdeptrai.notion.site/MOCK-project-170f363afe3980b6bc66f2a9bb8b923d?pvs=4)
