// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
//
// Created by andrew on 11/30/23.
//

#include "filesystem/journal/journal.h"

#include "filesystem/journal/structs/transaction.h"
#include "filesystem/journal/structs/journal_commit_block.h"
#include "filesystem/journal/structs/journal_descriptor_block.h"
#include "filesystem/journal/structs/journal_block_header.h"

#include "filesystem/structs/iterators/inode_iterator.h"
#include "filesystem/journal/structs/iterators/descriptor_block_iterator.h"

#include <fmt/format.h>
#include <iostream>


Journal::Journal(const FSInfo *fs_info) : fs_info(fs_info) {
    journal_inode = Inode(fs_info, fs_info->super_block->s_journal_inum);
    auto journal_head = fs_info->get_block(journal_inode.get_block(0));

    journal_superblock = JournalSuperblock(reinterpret_cast<journal_superblock_s *>(journal_head));
}

void Journal::print(bool superblock, bool transactions) const {
    if (superblock) {
        std::cout << "Superblock:" << std::endl;
        this->journal_superblock.print();
        std::cout << std::endl;
    }
    if (transactions) {
        std::cout << "Transactions:" << std::endl;
        Transaction::print_header();
        for (auto &transaction: get_transactions()) {
            transaction.print();
        }
    }
}

std::vector<Transaction> Journal::get_transactions() const {
    std::vector<Transaction> transactions;
    Transaction current_transaction;
    block_t journal_block = 0;

    for (auto it = journal_inode.begin(); it != journal_inode.end(); ++it) {
        auto block = *it;
        auto block_ptr = fs_info->get_block<journal_header_s>(block);
        if (swap_endian(block_ptr->h_magic) != JBD2_MAGIC_NUMBER) {
            if (current_transaction.is_empty()) {
                break;
            }
        }
        auto block_header = JournalHeader(block_ptr);
        auto block_type = block_header.get_type();

        switch (block_type) {
            case JBD2_DESCRIPTOR_BLOCK: {
                JournalDescriptorBlock desc_block(block_ptr, swap_endian(journal_superblock->s_feature_incompat));
                size_t n_tags = 0;
                for (auto tag: desc_block) {
                    auto block_num = tag.get_block_num();
                    current_transaction.add_block(block_num, tag.get_flags());
                    n_tags++;
                }
                current_transaction.set_first_j_block(journal_block + 1);
                journal_block += n_tags;
                it += n_tags;
                break;
            }
            case JBD2_COMMIT_BLOCK: {
                auto commit_block = JournalCommitBlock(reinterpret_cast<const commit_header *>(block_ptr));
                current_transaction.set_timestamp(commit_block.get_timestamp());
                current_transaction.set_sequence(commit_block.get_sequence());
                transactions.emplace_back(current_transaction);
                current_transaction = Transaction();
                break;
            }
            case JBD2_REVOKE_BLOCK: {
                // MAYBE: Add support for revoke blocks
                break;
            }
            default:
                break;
        }
        journal_block++;
    }
    return transactions;
}
