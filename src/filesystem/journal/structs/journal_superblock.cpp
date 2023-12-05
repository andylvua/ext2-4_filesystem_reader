//
// Created by andrew on 11/30/23.
//

#include "filesystem/journal/structs/journal_superblock.h"
#include "utils/utils.h"

#include <iostream>


JournalSuperblock::JournalSuperblock(journal_superblock_s *superblock) : superblock(superblock) {
    auto superblock_header = reinterpret_cast<const journal_header_s *>(superblock);
    type = swap_endian(superblock_header->h_blocktype);
}

journal_superblock_s *JournalSuperblock::operator->() const {
    return superblock;
}

void JournalSuperblock::print() const {
    std::stringstream superblock_info;

    superblock_info << "Journal device blocksize: " << swap_endian(superblock->s_blocksize) << std::endl;
    superblock_info << "Total journal blocks: " << swap_endian(superblock->s_maxlen) << std::endl;
    superblock_info << "First block of log: " << swap_endian(superblock->s_first) << std::endl;
    superblock_info << "Commit ID expected: " << swap_endian(superblock->s_sequence) << std::endl;
    superblock_info << "Log start block: " << swap_endian(superblock->s_start) << std::endl;
    superblock_info << "Last error value: " << swap_endian(superblock->s_errno) << std::endl;
    if (type == JBD2_SUPERBLOCK_V1) {
        std::cout << superblock_info.str();
        return; // the remaining fields are only valid for v2
    }
    superblock_info << "Compatible feature set: ";
    superblock_info << format_flags(jbd2_compat_feature_map, swap_endian(superblock->s_feature_compat)) << std::endl;
    superblock_info << "Incompatible feature set: ";
    superblock_info << format_flags(jbd2_incompat_feature_map, swap_endian(superblock->s_feature_incompat))
                    << std::endl;
    superblock_info << "Read-only compatible feature set: ";
    superblock_info << format_flags(jbd2_ro_compat_feature_map, swap_endian(superblock->s_feature_ro_compat))
                    << std::endl;
    superblock_info << "Journal UUID: " << format_uuid(superblock->s_uuid) << std::endl;
    superblock_info << "Number of FS shares: " << swap_endian(superblock->s_nr_users) << std::endl;
    std::cout << superblock_info.str();
}
