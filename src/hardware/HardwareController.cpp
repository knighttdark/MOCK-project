#include "hardware/HardwareController.h"
#include "controller/PlayingMediaController.h"
#include "model/PlayingMediaModel.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <taglib/fileref.h>

Hardware::Hardware(const string& portName, unsigned int baudRate)
    : serial(io, portName) {
    serial.set_option(serial_port_base::baud_rate(baudRate));
    serial.set_option(serial_port_base::character_size(8));
    serial.set_option(serial_port_base::parity(serial_port_base::parity::none));
    serial.set_option(serial_port_base::stop_bits(serial_port_base::stop_bits::one));
    serial.set_option(serial_port_base::flow_control(serial_port_base::flow_control::none));
}


Hardware::~Hardware() {
    if (serial.is_open()) {
        serial.close();
        cout << "Serial port closed." << endl;
    }
}


void Hardware::startListening() {
    boost::asio::streambuf buffer;
    while (true) {
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

void Hardware::handleCommand(string& command) {
    command.erase(remove(command.begin(), command.end(), '\r'), command.end());
    command.erase(remove(command.begin(), command.end(), '\n'), command.end());
    PlayingMediaController* playingController = dynamic_cast<PlayingMediaController*>(
                ManagerController::getInstance().getController("PlayingView"));

    if (command.rfind("__", 0) == 0) {
        command = command.substr(2);
    }

    if (!command.empty() && all_of(command.begin(), command.end(), ::isdigit)) {
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
        if (command == "s") {
            playingController->stop();
        } else if (command == "p") {
            playingController->skipToPrevious();
        } else if (command == "c") {
            if (Mix_PausedMusic()) {
                Mix_ResumeMusic();
                playingController->setIsPlaying(true);
            } else {
                Mix_PauseMusic();
                playingController->setIsPlaying(false);
            }
        } else if (command == "n") {
            playingController->skipToNext();
        } else {
            cerr << "Unknown character command: " << command << "\n";
        }
    } else {
        cerr << "Invalid or empty command received." << endl;
    }
}