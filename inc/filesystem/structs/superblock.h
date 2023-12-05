// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
//
// Created by andrew on 11/24/23.
//

#ifndef FS_READER_SUPERBLOCK_H
#define FS_READER_SUPERBLOCK_H

#include "disk_image/disk_image.h"
#include "utils/utils.h"
#include "filesystem/types.h"

#include <sstream>
#include <stdexcept>
#include <iostream>
#include <ext2fs/ext2_fs.h>


class Superblock {
public:
    explicit Superblock(const FilesystemDump &dump);

    void print() const;

    ext2_super_block* operator->() const;

    block_size_t get_block_size() const;

private:
    ext2_super_block *data;
};

#endif //FS_READER_SUPERBLOCK_H
