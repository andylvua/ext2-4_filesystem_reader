// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
//
// Created by andrew on 11/30/23.
//

#include "disk_image/disk_image_dump.h"
#include "errors/exception.h"

#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>


ImageDump::ImageDump(std::string path) : path(std::move(path)) {
    dump();
}

ImageDump::~ImageDump() {
    munmap(dump_data, dump_size);
}

[[nodiscard]] size_t ImageDump::size() const {
    return dump_size;
}

auto *ImageDump::operator->() {
    return data();
}

void ImageDump::dump() {
    int fd = open(path.c_str(), O_RDONLY);
    if (fd == -1) {
        throw FSReaderException("can't open filesystem image: " + path, errno);
    }

    struct stat sb{};
    if (fstat(fd, &sb) == -1) {
        throw FSReaderException("can't get filesystem image size: " + path, errno);
    }

    dump_size = sb.st_size;

    dump_data = static_cast<char *>(mmap(nullptr, dump_size, PROT_READ, MAP_PRIVATE, fd, 0));
    close(fd);

    if (dump_data == MAP_FAILED) {
        throw FSReaderException("can't read filesystem image: " + path, errno);
    }
}
