#ifndef EXCEPTION_H
#define EXCEPTION_H

#include <exception>
#include <string>
#include <limits>

class InputException : public std::exception {
private:
    std::string message;

public:
    explicit InputException(const std::string& msg) : message(msg) {}

    const char* what() const noexcept override {
        return message.c_str();
    }
};

#endif // EXCEPTION_H
