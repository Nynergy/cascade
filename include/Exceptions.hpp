#pragma once

struct InvalidFileException : public std::exception {

private:
    const char * message;

public:
    InvalidFileException(const char * messageIn) : message(messageIn) {}
    const char * what() {
        return message;
    }

};
