#ifndef HARDWARECONTROLLER_H
#define HARDWARECONTROLLER_H

#include <iostream>
#include <boost/asio.hpp>
#include <boost/asio/streambuf.hpp>
#include <boost/asio/read_until.hpp>
#include <sstream>

using namespace boost::asio;
using namespace std;

class Hardware {
public:
    // Constructor và Destructor
    Hardware(const string& portName, unsigned int baudRate);
    ~Hardware();

    // Phương thức public
    void startListening();
    void handleCommand(string& command);

private:
    // Thành viên private
    io_context io;
    serial_port serial;
    //PlayingMediaController playingController; // Đối tượng điều khiển âm thanh
    int currentVolume = 50; // Giá trị âm lượng hiện tại (mặc định là 50)
};

#endif // HARDWARECONTROLLER_H