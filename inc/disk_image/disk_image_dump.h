//
// Created by andrew on 11/26/23.
//

#ifndef FS_READER_DISK_IMAGE_DUMP_H
#define FS_READER_DISK_IMAGE_DUMP_H

#include <vector>
#include <string>


class ImageDump {
public:
    using data_t = std::vector<char>;

public:
    explicit ImageDump(std::string path);

    [[nodiscard]] size_t size() const;

    template<typename T = char>
    [[nodiscard]] T *data() {
        return reinterpret_cast<T *>(dump_data.data());
    }

    template<typename T = char>
    [[nodiscard]] T *data(const size_t offset) {
        return reinterpret_cast<T *>(dump_data.data() + offset);
    }

    auto *operator->();

private:
    void dump();

    std::string path;
    data_t dump_data;
};

#endif //FS_READER_DISK_IMAGE_DUMP_H
