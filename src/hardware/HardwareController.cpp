#include "hardware/HardwareController.h"
#include "controller/PlayingMediaController.h"
#include "model/PlayingMediaModel.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <taglib/fileref.h>

/* Constructor: Initializes the hardware communication via the serial port */
Hardware::Hardware(const string& portName, unsigned int baudRate)
    : serial(io, portName) {
    serial.set_option(serial_port_base::baud_rate(baudRate));
    serial.set_option(serial_port_base::character_size(8));
    serial.set_option(serial_port_base::parity(serial_port_base::parity::none));
    serial.set_option(serial_port_base::stop_bits(serial_port_base::stop_bits::one));
    serial.set_option(serial_port_base::flow_control(serial_port_base::flow_control::none));
}

/* Destructor: Ensures the serial port is closed properly */
Hardware::~Hardware() {
    if (serial.is_open()) {
        serial.close();
        cout << "Serial port closed." << endl;
    }
}

/* Start listening for commands from the hardware */
void Hardware::startListening() {
    boost::asio::streambuf buffer;

    while (true) {
        /* Read commands from the serial port */
        size_t n = read_until(serial, buffer, "\n");
        istream is(&buffer);
        string command;
        command.resize(n);
        is.read(&command[0], n);
        command = command.substr(0, is.gcount());
        handleCommand(command);
        buffer.consume(n);
    }
}

/* Handle incoming commands from the hardware */
void Hardware::handleCommand(string& command) {
    /* Remove carriage return and newline characters from the command */
    command.erase(remove(command.begin(), command.end(), '\r'), command.end());
    command.erase(remove(command.begin(), command.end(), '\n'), command.end());

    /* Get the PlayingMediaController instance to perform actions */
    PlayingMediaController* playingController = dynamic_cast<PlayingMediaController*>(
                ManagerController::getInstance().getController("PlayingView"));

    /* Remove the "__" prefix if present */
    if (command.rfind("__", 0) == 0) {
        command = command.substr(2);
    }

    if (!command.empty() && all_of(command.begin(), command.end(), ::isdigit)) {
        /* Convert the command to a number and adjust the volume */
        try {
            int rawValue = stoi(command);
            int mappedValue = rawValue * 100 / 4095;
            if (abs(mappedValue - currentVolume) >= 2) {
                playingController->adjustVolume(mappedValue);
                currentVolume = mappedValue;
            } else {
                /* do nothing */
            }
        } catch (const exception& e) {
            cerr << "Error converting command to number: " << e.what() << "\n";
        }
    } else if (!command.empty()) {
        /* Handle character-based commands for playback control */
        if (command == "s") {
            /* Stop the current playback */
            playingController->stop();
        } else if (command == "p") {
            /* Skip to the previous track */
            playingController->skipToPrevious();
        } else if (command == "c") {
            /* Pause or resume the current media */
            if (Mix_PausedMusic()) {
                Mix_ResumeMusic();
                playingController->setIsPlaying(true);
            } else {
                Mix_PauseMusic();
                playingController->setIsPlaying(false);
            }
        } else if (command == "n") {
            /* Skip to the next track */
            playingController->skipToNext();
        } else {
            cerr << "Unknown character command: " << command << "\n";
        }
    } else {
        cerr << "Invalid or empty command received." << endl;
    }
}