// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
//
// Created by andrew on 11/24/23.
//

#ifndef FS_READER_FS_READER_H
#define FS_READER_FS_READER_H

#include "utils/utils.h"
#include "disk_image/disk_image.h"
#include "filesystem/structs/superblock.h"
#include "filesystem/structs/descriptor_table.h"
#include "filesystem/filesystem.h"
#include "filesystem/structs/inode.h"
#include "filesystem/structs/directory.h"

#include <sys/stat.h>
#include <fmt/format.h>


static constexpr auto ROOT_HEADER = \
"---------------------------------------ROOT---------------------------------------\n";

static constexpr auto SUPERBLOCK_HEADER = \
"---------------------------------------SUPERBLOCK---------------------------------------\n";

static constexpr auto JOURNAL_HEADER = \
"---------------------------------------JOURNAL---------------------------------------\n";



class FS_Reader {
public:
    explicit FS_Reader(const FilesystemDump &dump) : filesystem(dump) {}

    void print_root(bool recursive = false) const {
        std::cout << ROOT_HEADER << std::endl;
        auto &fs_info = filesystem.fs_info;
        Dirent::print_header();
        Directory(&fs_info, Inode(&fs_info, EXT2_ROOT_INO), "/").print(recursive);
        std::cout << std::endl;
    }

    void print_superblock() const {
        std::cout << SUPERBLOCK_HEADER << std::endl;
        filesystem.fs_info.super_block.print();
    }

    void print_journal(bool superblock = false, bool transactions = false) const {
        std::cout << JOURNAL_HEADER << std::endl;
        if (!filesystem.has_journal) {
            std::cerr << "fs_reader: Error: No journal found" << std::endl;
            return;
        }
        filesystem.journal.print(superblock, transactions);
    }

    Filesystem filesystem;
};


#endif //FS_READER_FS_READER_H
