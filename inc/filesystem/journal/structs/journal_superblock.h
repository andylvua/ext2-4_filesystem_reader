// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
//
// Created by andrew on 11/30/23.
//

#ifndef FS_READER_JOURNAL_SUPERBLOCK_H
#define FS_READER_JOURNAL_SUPERBLOCK_H

#include "filesystem/journal/jbd2.h"

#include <cstdint>


class JournalSuperblock {
public:
    JournalSuperblock() = default;

    explicit JournalSuperblock(journal_superblock_s *superblock);

    journal_superblock_s *operator->() const;

    void print() const;

private:
    journal_superblock_s *superblock = nullptr;
    uint32_t type = JBD2_SUPERBLOCK_V2;
};

#endif //FS_READER_JOURNAL_SUPERBLOCK_H
