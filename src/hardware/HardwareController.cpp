#include "hardware/HardwareController.h"

// Constructor
Hardware::Hardware(const string& portName, unsigned int baudRate)
    : serial(io, portName) {
    serial.set_option(serial_port_base::baud_rate(baudRate));
    serial.set_option(serial_port_base::character_size(8));
    serial.set_option(serial_port_base::parity(serial_port_base::parity::none));
    serial.set_option(serial_port_base::stop_bits(serial_port_base::stop_bits::one));
    serial.set_option(serial_port_base::flow_control(serial_port_base::flow_control::none));

    cout << "Serial port initialized on " << portName << " with baud rate " << baudRate << endl;
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
    cout << "Waiting for input from user..." << endl;

    boost::asio::streambuf buffer;
    while (true) {
        // Đọc dữ liệu từ cổng nối tiếp
        size_t n = read_until(serial, buffer, "\n");

        // Lấy dữ liệu từ buffer và chuyển thành chuỗi
        istream is(&buffer);
        string command;
        command.resize(n);
        is.read(&command[0], n);

        // Xử lý lệnh nhận được
        handleCommand(command);

        // Xóa dữ liệu đã xử lý khỏi buffer
        buffer.consume(n);
    }
}

// Phương thức handleCommand
void Hardware::handleCommand(string& command) {
    command.erase(remove(command.begin(), command.end(), '\r'), command.end());
    command.erase(remove(command.begin(), command.end(), '\n'), command.end());

    if (command == "s") {
        cout << "Stop Music" << endl;
        // exit(0);
    }
    else if (command == "p") {
        cout << "Previous Song" << endl;
    }
    else if (command == "c") {
        cout << "Continue Playing Music" << endl;
    }
    else if (command == "n") {
        cout << "Next Song" << endl;
    }
    else {
        cout << "Unknown Command: " << command << endl;
    }
}
