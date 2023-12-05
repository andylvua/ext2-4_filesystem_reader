//
// Created by andrew on 11/25/23.
//

#ifndef DIRENT_H
#define DIRENT_H

#include "inode.h"
#include "utils/utils.h"
#include "filesystem/filesystem_info.h"
#include "filesystem/types.h"

#include <string>
#include <ext2fs/ext2_fs.h>
#include <fmt/format.h>
#include <sys/stat.h>


class Dirent {
public:
    Dirent() = default;

    Dirent(const FSInfo *fs_info, ext2_dir_entry *entry);

    Dirent(const FSInfo *fs_info, inode_block_t inode_num, std::string path);

    [[nodiscard]] std::string get_name() const;

    [[nodiscard]] int get_type() const;

    [[nodiscard]] bool is_directory() const;

    [[nodiscard]] bool is_valid() const;

    [[nodiscard]] Inode get_inode() const;

    block_size_t next();

    static void print_header();

    void print() const;

    friend bool operator==(const Dirent &lhs, const Dirent &rhs);

private:
    const FSInfo *fs_info = nullptr;
    ext2_dir_entry *entry = nullptr;
    Inode inode;
    std::string name;
};

#endif //DIRENT_H
