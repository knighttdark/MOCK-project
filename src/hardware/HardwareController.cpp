#include "hardware/HardwareController.h"
#include "controller/PlayingMediaController.h"
#include "model/PlayingMediaModel.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <taglib/fileref.h>

// Constructor
Hardware::Hardware(const string& portName, unsigned int baudRate)
    : serial(io, portName) {
    serial.set_option(serial_port_base::baud_rate(baudRate));
    serial.set_option(serial_port_base::character_size(8));
    serial.set_option(serial_port_base::parity(serial_port_base::parity::none));
    serial.set_option(serial_port_base::stop_bits(serial_port_base::stop_bits::one));
    serial.set_option(serial_port_base::flow_control(serial_port_base::flow_control::none));

    // cout << "Serial port initialized on " << portName << " with baud rate " << baudRate << endl;
}

// Destructor
Hardware::~Hardware() {
    if (serial.is_open()) {
        serial.close();
        cout << "Serial port closed." << endl;
    }
}

// Phương thức startListening
void Hardware::startListening() {
    // cout << "Waiting for input from user..." << endl;

    boost::asio::streambuf buffer;
    while (true) {
        // Đọc dữ liệu từ cổng nối tiếp
        size_t n = read_until(serial, buffer, "\n");

        // Lấy dữ liệu từ buffer và chuyển thành chuỗi
        istream is(&buffer);
        string command;
        command.resize(n);
        is.read(&command[0], n);

        command = command.substr(0, is.gcount());

        // Xử lý lệnh nhận được
        handleCommand(command);

        // Xóa dữ liệu đã xử lý khỏi buffer
        buffer.consume(n);
    }
}

void Hardware::handleCommand(string& command) {
    // Loại bỏ ký tự xuống dòng và khoảng trắng
    command.erase(remove(command.begin(), command.end(), '\r'), command.end());
    command.erase(remove(command.begin(), command.end(), '\n'), command.end());
    command.erase(remove(command.begin(), command.end(), ' '), command.begin());

    // static PlayingMediaController playingController;

                // Lấy PlayingMediaController từ ManagerController
            PlayingMediaController* playingController = dynamic_cast<PlayingMediaController*>(
                ManagerController::getInstance().getController("PlayingView"));



    // Kiểm tra nếu dữ liệu bắt đầu bằng '__'
    if (command.rfind("__", 0) == 0) {
        command = command.substr(2); // Loại bỏ '__' ở đầu chuỗi
    }

    // Nếu command chứa dấu `__` ở giữa
    size_t pos = command.find("__");
    if (pos != string::npos) {
        command = command.substr(0, pos); // Lấy phần trước `__`
    }

    // Xử lý nếu command chỉ chứa số
    if (!command.empty() && all_of(command.begin(), command.end(), ::isdigit)) {
        try {
            int rawValue = stoi(command); // Chuyển chuỗi thành số nguyên
            // cout << "Processed numeric value: " << rawValue << "\n";

            // Nội suy từ 0-4095 sang 0-100
            int mappedValue = rawValue * 100 / 4095;

            // Kiểm tra nếu thay đổi âm lượng vượt quá 10
            if (abs(mappedValue - currentVolume) >= 4) {
                playingController->adjustVolume(mappedValue); // Gọi hàm điều chỉnh âm lượng
                currentVolume = mappedValue; // Cập nhật giá trị âm lượng hiện tại
            } else {
                // cout << "Volume change too small: " << mappedValue
                //      << " (current: " << currentVolume << ")\n";
            }
        } catch (const std::exception& e) {
            cerr << "Error converting command to number: " << e.what() << "\n";
        }
    } else if (!command.empty()) {
        // Xử lý nếu command là chuỗi ký tự
        if (command == "ss") {
            playingController->stop();
            // cout << "Stopping music...\n";
        } else if (command == "pp") {
            playingController->skipToPrevious();
            // cout << "Skipping to previous song...\n";
        } else if (command == "cc") {
            if (Mix_PausedMusic()) {
                Mix_ResumeMusic();
                playingController->setIsPlaying(true);
                // cout << "Resuming music...\n";
            } else {
                Mix_PauseMusic();
                playingController->setIsPlaying(false);
                // cout << "Pausing music...\n";
            }
        } else if (command == "nn") {
            playingController->skipToNext();
            // cout << "Skipping to next song...\n";
        } else {
            cerr << "Unknown character command: " << command << "\n";
        }
    } else {
        cerr << "Invalid or empty command received." << endl;
    }
}