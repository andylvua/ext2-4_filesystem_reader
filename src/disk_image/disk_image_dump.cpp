//
// Created by andrew on 11/30/23.
//

#include "disk_image/disk_image_dump.h"
#include "errors/exception.h"

#include <fstream>
#include <stdexcept>
#include <cstring>


ImageDump::ImageDump(std::string path) : path(std::move(path)) {
    dump();
}

[[nodiscard]] size_t ImageDump::size() const {
    return dump_data.size();
}

auto *ImageDump::operator->() {
    return data();
}


void ImageDump::dump() {
    std::ifstream file(path);

    if (!file.is_open()) {
        throw FSReaderException("Can't open filesystem image: " + path, errno);
    }

    if (!file.eof() && !file.fail()) {
        file.unsetf(std::ios::skipws);

        file.seekg(0, std::ios::end);
        const std::streampos file_size = file.tellg();
        file.seekg(0, std::ios::beg);

        dump_data.reserve(file_size);
        dump_data.resize(file_size);
        file.read(dump_data.data(), file_size);
    } else {
        throw FSReaderException("Can't read filesystem image: " + path, errno);
    }

    file.close();
}
