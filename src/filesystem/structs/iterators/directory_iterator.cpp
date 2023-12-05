// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
//
// Created by andrew on 11/30/23.
//

#include "filesystem/structs/iterators/directory_iterator.h"


DirectoryIterator::DirectoryIterator(const FSInfo *fs_info, InodeIterator block) : fs_info(fs_info), block(block) {
    if (*block == 0) {
        dirent = Dirent();
        return;
    }
    if (block.get_inode()->has_inline_data()) {
        // Construct a fake dirent for the inline data
        dirent = Dirent(fs_info, *reinterpret_cast<inode_block_t *>(block.get_inode()->get_inline_data()), "..");
        return;
    }
    dirent = Dirent(fs_info, fs_info->get_block<ext2_dir_entry>(*block));
}

void DirectoryIterator::next_block() {
    offset = 0;
    ++block;
    if (*block == 0) {
        dirent = Dirent();
        return;
    }
    dirent = Dirent(fs_info, fs_info->get_block<ext2_dir_entry>(*block));
}

void DirectoryIterator::next_inline_entry() {
    if (offset == 0) {
        offset = sizeof(inode_block_t);
        auto entry_p = block.get_inode()->get_inline_data() + offset;
        dirent = Dirent(fs_info, reinterpret_cast<ext2_dir_entry *>(entry_p));
        if (!dirent.is_valid()) {
            dirent = Dirent();
        }
        return;
    }
    auto inline_data_size = block.get_inode()->get_size();
    do {
        offset += dirent.next();
    } while (!dirent.is_valid() && offset < inline_data_size);
    if (offset >= inline_data_size) {
        dirent = Dirent();
    }
}

DirectoryIterator &DirectoryIterator::operator++() {
    if (block.get_inode()->has_inline_data()) {
        this->next_inline_entry();
        return *this;
    }

    do {
        do {
            offset += dirent.next();
        } while (!dirent.is_valid() && offset < fs_info->block_size);
        if (offset < fs_info->block_size) {
            return *this;
        }
        this->next_block();
    } while (!dirent.is_valid() && *block != 0);

    return *this;
}

bool DirectoryIterator::operator==(const DirectoryIterator &other) const {
    return dirent == other.dirent;
}

bool DirectoryIterator::operator!=(const DirectoryIterator &other) const {
    return !(*this == other);
}

DirectoryIterator::reference DirectoryIterator::operator*() {
    return dirent;
}
