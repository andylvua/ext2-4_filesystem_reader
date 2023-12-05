// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
//
// Created by andrew on 11/29/23.
//

#ifndef FS_READER_JOURNAL_H
#define FS_READER_JOURNAL_H

#include "filesystem/filesystem_info.h"
#include "filesystem/structs/inode.h"

#include "filesystem/journal/structs/journal_superblock.h"
#include "filesystem/journal/structs/transaction.h"


#include <vector>


class Journal {
public:
    Journal() = default;

    explicit Journal(const FSInfo *fs_info);

    void print(bool superblock, bool transactions) const;

private:
    [[nodiscard]] std::vector<Transaction> get_transactions() const;

    const FSInfo *fs_info = nullptr;
    JournalSuperblock journal_superblock;
    Inode journal_inode;
};

#endif //FS_READER_JOURNAL_H
