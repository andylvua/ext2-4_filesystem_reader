// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
//
// Created by andrew on 11/25/23.
//

#ifndef DESC_TABLE_H
#define DESC_TABLE_H

#include "disk_image/disk_image.h"
#include "filesystem/types.h"
#include "superblock.h"

#include <ext2fs/ext2_fs.h>


class DescriptorTable {
public:
    DescriptorTable(const FilesystemDump &dump, const Superblock &superblock);

    [[nodiscard]] ext2_group_desc* get_group_desc(uint32_t group_num) const;

private:
    ext2_group_desc* group_desc_table;
    block_size_t desc_size = sizeof(ext2_group_desc);
};

#endif //DESC_TABLE_H
