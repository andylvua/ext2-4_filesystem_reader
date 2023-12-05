//
// Created by andrew on 11/30/23.
//

#include "disk_image/disk_image.h"
#include "errors/exception.h"

#include <stdexcept>
#include <iostream>


DiskImage::DiskImage(const std::string &path) : dump(std::make_shared<ImageDump>(path)),
                                         partition_table(dump) {
    std::cout << "Path: " << path << '\n';
}

void DiskImage::print_partitions() const {
    partition_table.print_partitions();
}

FilesystemDump DiskImage::get_partition(const size_t partition_num) const {
    return partition_table.get_partition(partition_num);
}

FilesystemDump DiskImage::to_filesystem() const {
    if (partition_table.has_partitions()) {
        std::cout << "Partition table found\n";
        print_partitions();
        throw FSReaderException("Cannot dump partitioned disk image to a single filesystem");
    }
    return FilesystemDump(dump, 0, dump->size());
}
