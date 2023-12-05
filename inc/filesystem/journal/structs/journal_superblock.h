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
