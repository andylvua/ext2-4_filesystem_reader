//
// Created by andrew on 12/3/23.
//

#ifndef FS_READER_EXCEPTION_H
#define FS_READER_EXCEPTION_H

#include <stdexcept>
#include <string>
#include <source_location>
#include <cstring>
#include <utility>


class FSReaderException : public std::exception {
public:
    explicit FSReaderException(std::string msg, int error = 0) :
            msg(std::move(msg)),
            error(error) {}


    [[nodiscard]] std::string what() {
        std::string what = "fs_reader: ";
        what += msg;
        if (error) {
            what += ": " + std::string(strerror(error));
        }
        return what;
    }

private:
    std::string msg;
    int error;
};

#endif //FS_READER_EXCEPTION_H
