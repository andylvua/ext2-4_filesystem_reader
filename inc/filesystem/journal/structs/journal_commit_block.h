//
// Created by andrew on 11/30/23.
//

#ifndef FS_READER_JOURNAL_COMMIT_BLOCK_H
#define FS_READER_JOURNAL_COMMIT_BLOCK_H

#include "filesystem/journal/jbd2.h"
#include "utils/utils.h"

#include <cstdint>


class JournalCommitBlock {
public:
    JournalCommitBlock() = default;

    explicit JournalCommitBlock(const commit_header *header);

    [[nodiscard]] uint64_t get_timestamp() const;

    [[nodiscard]] uint32_t get_sequence() const;

private:
    const commit_header *header = nullptr;
};

#endif //FS_READER_JOURNAL_COMMIT_BLOCK_H
