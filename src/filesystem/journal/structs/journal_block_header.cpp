// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
//
// Created by andrew on 11/30/23.
//

#include "filesystem/journal/structs/journal_block_header.h"
#include "utils/utils.h"


JournalHeader::JournalHeader(const journal_header_s *header) : header(header) {}

void JournalHeader::print() const {
    std::cout << "Journal header:" << std::endl;
    std::cout << "Journal magic: " << swap_endian(header->h_magic) << std::endl;
    std::cout << "Journal blocktype: " << swap_endian(header->h_blocktype) << std::endl;
    std::cout << "Journal sequence: " << swap_endian(header->h_sequence) << std::endl;
}

uint32_t JournalHeader::get_type() const {
    return swap_endian(header->h_blocktype);
}
