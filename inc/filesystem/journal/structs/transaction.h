//
// Created by andrew on 11/30/23.
//

#ifndef FS_READER_TRANSACTION_H
#define FS_READER_TRANSACTION_H

#include "filesystem/journal/jbd2.h"
#include "utils/utils.h"
#include "filesystem/types.h"


#include <cstdint>
#include <vector>


class Transaction {
public:
    void set_timestamp(uint64_t transaction_timestamp);

    void set_sequence(uint32_t transaction_sequence);

    void add_block(block_t fs_block, uint32_t flags);

    void set_first_j_block(block_t j_block);

    [[nodiscard]] bool is_empty() const;

    static void print_header();

    void print() const;

private:
    uint64_t timestamp{};
    uint32_t sequence{};

    struct block {
        block_t fs_block;
        block_t j_block;
        uint32_t flags;
    };
    std::vector<block> blocks{};
};

#endif //FS_READER_TRANSACTION_H
