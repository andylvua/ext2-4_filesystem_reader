//
// Created by andrew on 11/30/23.
//

#ifndef FS_READER_JOURNAL_DESCRIPTOR_TAG_H
#define FS_READER_JOURNAL_DESCRIPTOR_TAG_H

#include "filesystem/journal/jbd2.h"
#include "filesystem/types.h"

#include <cstdint>


class DescriptorTag {
public:
    DescriptorTag() = default;

    explicit DescriptorTag(const char *tag, uint32_t incompat_flags);

    [[nodiscard]] block_t get_block_num() const;

    [[nodiscard]] uint32_t get_flags() const;

    void print() const;

    friend bool operator==(const DescriptorTag &lhs, const DescriptorTag &rhs);

private:
    union {
        const journal_block_tag_s *tag;
        const journal_block_tag3_s *tag3;
    } tag{};

    uint32_t incompat_flags = 0;
};

#endif //FS_READER_JOURNAL_DESCRIPTOR_TAG_H
