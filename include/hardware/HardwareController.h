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
    
    Hardware(const string& portName, unsigned int baudRate);
    ~Hardware();

    
    void startListening();
    void sendCommandToBoard(const string& command);
    void handleCommand(string& command);

private:
    
    io_context io;
    serial_port serial;
    
    int currentVolume = 50; 
};

#endif 
