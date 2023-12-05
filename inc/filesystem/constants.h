// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
//
// Created by andrew on 12/2/23.
//

#ifndef FS_READER_CONSTANTS_H
#define FS_READER_CONSTANTS_H

#include <ext2fs/ext2_fs.h>

/* Inode constants */
static constexpr auto IBLOCK_SIZE = sizeof(ext2_inode::i_block);
static constexpr auto XATTR_DATA_IDX = 7;

/* Image constants */
static constexpr auto SECTOR_SIZE = 512; // Logical sector size on image

#endif //FS_READER_CONSTANTS_H
