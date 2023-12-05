// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
//
// Created by andrew on 11/30/23.
//

#include "filesystem/journal/structs/transaction.h"

#include <fmt/format.h>


void Transaction::set_timestamp(uint64_t transaction_timestamp) {
    this->timestamp = transaction_timestamp;
}

void Transaction::set_sequence(uint32_t transaction_sequence) {
    this->sequence = transaction_sequence;
}

void Transaction::add_block(block_t fs_block, uint32_t flags) {
    blocks.emplace_back(fs_block, 0, flags);
}

void Transaction::set_first_j_block(block_t j_block) {
    for (auto &block: blocks) {
        if (block.j_block != 0) {
            continue;
        }
        block.j_block = j_block++;
    }
}

bool Transaction::is_empty() const {
    return blocks.empty();
}

void Transaction::print_header() {
    fmt::print("{:10}    {:<10}  {:25}  {}\n", "FS Block", "J Block", "Timestamp", "Sequence");
}

void Transaction::print() const {
    for (auto block: blocks) {
        fmt::print("{:10} -> {:<10}  {:25}  {}\n",
                   block.fs_block, block.j_block, format_time(timestamp), sequence);
    }
}
