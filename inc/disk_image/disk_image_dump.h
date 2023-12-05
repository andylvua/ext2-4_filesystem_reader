//
// Created by andrew on 11/26/23.
//

#ifndef FS_READER_DISK_IMAGE_DUMP_H
#define FS_READER_DISK_IMAGE_DUMP_H

#include <vector>
#include <string>


class ImageDump {
public:
    ImageDump(const ImageDump &) = delete;

    ImageDump &operator=(const ImageDump &) = delete;

    ImageDump(ImageDump &&) noexcept = delete;

    ImageDump &operator=(ImageDump &&) noexcept = delete;

    explicit ImageDump(std::string path);

    ~ImageDump();

    [[nodiscard]] size_t size() const;

    template<typename T = char>
    [[nodiscard]] T *data() {
        return reinterpret_cast<T *>(dump_data);
    }

    template<typename T = char>
    [[nodiscard]] T *data(const size_t offset) {
        return reinterpret_cast<T *>(dump_data + offset);
    }

    auto *operator->();

private:
    void dump();

    std::string path;
    char *dump_data = nullptr;
    size_t dump_size = 0;
};

#endif //FS_READER_DISK_IMAGE_DUMP_H
