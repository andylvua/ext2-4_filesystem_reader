// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
//
// Created by andrew on 11/25/23.
//

#ifndef INODE_H
#define INODE_H

#include "disk_image/disk_image.h"
#include "filesystem/filesystem_info.h"
#include "filesystem/structs/iterators/decralations.h"
#include "filesystem/types.h"

#include <ext2fs/ext2_fs.h>
#include <ext2fs/ext3_extents.h>

using inode_block_t = uint32_t;


class Inode {
    using iterator = InodeIterator;
public:
    Inode() = default;

    explicit Inode(const FSInfo *fs_info, inode_block_t inode_num);

    [[nodiscard]] iterator begin() const;

    [[nodiscard]] iterator end() const;

    [[nodiscard]] block_t get_block(inode_block_t block_num) const;

    [[nodiscard]] size_t get_size() const;

    [[nodiscard]] bool is_extent() const;

    [[nodiscard]] bool has_inline_data() const;

    [[nodiscard]] bool is_valid() const;

    [[nodiscard]] char* get_inline_data();

    ext2_inode *operator->() const;

    friend bool operator==(const Inode &lhs, const Inode &rhs);

private:
    [[nodiscard]] block_t extent_to_block(ext3_extent_header *ex_header, inode_block_t block_num) const;

    [[nodiscard]] block_t logical_to_block(inode_block_t block_num) const;

    void read_inline_data();

    ext2_inode *inode = nullptr;
    block_t block = 0;
    data_t inline_data;

public:
    const FSInfo *fs_info = nullptr;
};

#endif //INODE_H
