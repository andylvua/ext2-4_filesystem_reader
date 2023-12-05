// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
//
// Created by andrew on 11/24/23.
//

#ifndef FS_READER_DISK_IMAGE_H
#define FS_READER_DISK_IMAGE_H

#include "disk_image_dump.h"
#include "disk_image/structs/partition_table.h"


#include <string>
#include <memory>


class DiskImage {
public:
    explicit DiskImage(const std::string &path);

    void print_partitions() const;

    [[nodiscard]] FilesystemDump get_partition(size_t partition_num) const;

    [[nodiscard]] FilesystemDump to_filesystem() const;

private:
    std::shared_ptr<ImageDump> dump;
    PartitionTable partition_table;
};

#endif //FS_READER_DISK_IMAGE_H
