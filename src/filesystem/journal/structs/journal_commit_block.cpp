// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
//
// Created by andrew on 11/30/23.
//

#include "filesystem/journal/structs/journal_commit_block.h"
#include "utils/utils.h"


JournalCommitBlock::JournalCommitBlock(const commit_header *header) : header(header) {}

uint64_t JournalCommitBlock::get_timestamp() const {
    return swap_endian(header->h_commit_sec);
}

uint32_t JournalCommitBlock::get_sequence() const {
    return swap_endian(header->h_sequence);
}
