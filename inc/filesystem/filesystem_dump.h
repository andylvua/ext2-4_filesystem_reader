//
// Created by andrew on 11/26/23.
//

#ifndef FS_DUMP_H
#define FS_DUMP_H

#include "disk_image/disk_image_dump.h"

#include <memory>


class FilesystemDump {
public:
    explicit FilesystemDump(std::shared_ptr<ImageDump> image, const size_t offset, const size_t size) :
            image(std::move(image)), dump_offset(offset), dump_size(size) {}

    template<typename T = char>
    [[nodiscard]] T *data() const {
        return image->data<T>(dump_offset);
    }

    template<typename T = char>
    [[nodiscard]] T *data(const size_t offset) const {
        return image->data<T>(dump_offset + offset);
    }

    [[nodiscard]] size_t size() const {
        return dump_size;
    }

private:
    std::shared_ptr<ImageDump> image;
    size_t dump_offset;
    size_t dump_size;
};

#endif //FS_DUMP_H
