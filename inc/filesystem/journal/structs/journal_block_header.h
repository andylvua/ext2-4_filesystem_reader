// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
//
// Created by andrew on 11/30/23.
//

#ifndef FS_READER_JOURNAL_BLOCK_HEADER_H
#define FS_READER_JOURNAL_BLOCK_HEADER_H

#include "filesystem/journal/jbd2.h"
#include "utils/utils.h"

#include <iostream>
#include <cstdint>


class JournalHeader {
public:
    JournalHeader() = default;

    explicit JournalHeader(const journal_header_s *header);

    void print() const;

    [[nodiscard]] uint32_t get_type() const;

private:
    const journal_header_s *header = nullptr;
};


#endif //FS_READER_JOURNAL_BLOCK_HEADER_H
