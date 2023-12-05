//
// Created by andrew on 11/29/23.
//

#ifndef FS_READER_FILESYSTEM_INFO_H
#define FS_READER_FILESYSTEM_INFO_H

#include "disk_image/disk_image.h"
#include "filesystem/structs/superblock.h"
#include "filesystem/structs/descriptor_table.h"
#include "utils/filesystem_type.h"
#include "types.h"

#include <iostream>


struct FSInfo {
    FilesystemDump dump;
    Superblock super_block;
    block_size_t block_size;
    DescriptorTable descriptor_table;
    FilesystemType type = FilesystemType::EXT2;

    explicit FSInfo(const FilesystemDump &dump) : dump(dump),
                                                  super_block(dump),
                                                  block_size(super_block.get_block_size()),
                                                  descriptor_table(dump, super_block),
                                                  type(get_fs_type(super_block)) {
        std::cout << "Filesystem type: " << get_fs_type_name(type) << "\n" << std::endl;
    }

    template<typename T = char>
    [[nodiscard]] T *get_data(const size_t offset = 0) const {
        return dump.data<T>(offset);
    }

    template<typename T = char>
    [[nodiscard]] T *get_block(const block_t block_num) const {
        return get_data<T>(block_num * block_size);
    }
};

#endif //FS_READER_FILESYSTEM_INFO_H
