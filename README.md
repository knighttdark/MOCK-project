# MOCK Project - Media Files Browser/Player

## Project Description

This is a CLI application for managing and playing music/video on Linux operating systems. The application allows users to browse and play media files (audio/video) from directories or external devices such as USB drives. Features include:

- **Browse media files** in a folder and all subfolders (with pagination support).
- **Playlists**: Manage playlists, allowing users to create, update, and delete them.
- **File metadata**: Display and edit metadata of audio and video files (using the Taglib library).
- **Music playback and control**: Play music using the SDL2 library, supporting Play/Pause, Next/Previous, volume control, and displaying playback time.
- **Connect to the S32K144 board** to control volume and display the currently playing song information.

## Links

- [Github Mock-proj](https://github.com/thanhetebkdn/MOCK-project)
- [Notion Mock-proj](https://thanhdeptrai.notion.site/MOCK-project-170f363afe3980b6bc66f2a9bb8b923d?pvs=4)

## Software Requirements

- **C/C++ compiler**: GCC or Clang.
- **SDL2 library**: Used for music playback.
- **Taglib library**: Used for processing file metadata.
- **S32K144 board**: Required for connecting and controlling functions such as volume and music playback.

## Installation and Usage

### Install Dependencies

Before running the application, you need to install the necessary libraries such as SDL2 and Taglib:

MOCK-project/
├── bin/ # Contains executable files after compilation
├── build/ # Build folder containing temporary files during compilation
├── include/ # Contains header files (.h)
├── src/ # Contains source files (.cpp)
├── tests/ # Contains unit test files for individual modules
├── Makefile # Makefile for building and linking the application
└── README.md # Documentation file for the project
