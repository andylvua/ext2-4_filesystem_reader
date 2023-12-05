// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
//
// Created by andrew on 11/25/23.
//

#ifndef FS_READER_DIRECTORY_H
#define FS_READER_DIRECTORY_H

#include "filesystem/filesystem.h"
#include "filesystem/structs/iterators/directory_iterator.h"
#include "inode.h"
#include "dirent.h"

#include <ext2fs/ext2_fs.h>
#include <utility>


class Directory {
    using iterator = DirectoryIterator;

public:
    Directory(const FSInfo *fs_info, const Inode& inode, std::string path = "/");

    [[nodiscard]] iterator begin() const;

    [[nodiscard]] iterator end() const;

    void print(bool recursive = false) const;

private:
    const FSInfo *fs_info;
    Inode inode;
    std::string path;
};


#endif //FS_READER_DIRECTORY_H
