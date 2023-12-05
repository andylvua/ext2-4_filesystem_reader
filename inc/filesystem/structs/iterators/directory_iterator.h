// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
//
// Created by andrew on 11/30/23.
//

#ifndef FS_READER_DIRECTORY_ITERATOR_H
#define FS_READER_DIRECTORY_ITERATOR_H

#include "filesystem/structs/dirent.h"
#include "filesystem/structs/iterators/inode_iterator.h"
#include "filesystem/types.h"


class DirectoryIterator {
    using value_type = Dirent;
    using pointer = Dirent *;
    using reference = Dirent &;

public:
    DirectoryIterator(const FSInfo *fs_info, InodeIterator block);

    DirectoryIterator &operator++();

    bool operator==(const DirectoryIterator &other) const;

    bool operator!=(const DirectoryIterator &other) const;

    reference operator*();

private:
    void next_block();

    void next_inline_entry();

    const FSInfo *fs_info;
    InodeIterator block;
    Dirent dirent;
    block_size_t offset = 0;
};

#endif //FS_READER_DIRECTORY_ITERATOR_H
