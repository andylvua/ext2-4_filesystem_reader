// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
//
// Created by andrew on 11/30/23.
//

#ifndef FS_READER_JOURNAL_DESCRIPTOR_BLOCK_H
#define FS_READER_JOURNAL_DESCRIPTOR_BLOCK_H

#include "filesystem/journal/jbd2.h"
#include "filesystem/journal/structs/iterators/declarations.h"
#include "utils/utils.h"
#include "journal_descriptor_tag.h"

#include <iostream>
#include <cstdint>


class JournalDescriptorBlock {
    using iterator = DescBlockIterator;

public:
    JournalDescriptorBlock() = default;

    JournalDescriptorBlock(const journal_header_s *header, uint32_t feature_incompat);

    [[nodiscard]] iterator begin() const;

    [[nodiscard]] iterator end() const;

    void print();

    [[nodiscard]] uint32_t get_tag_size() const;

    [[nodiscard]] uint32_t get_flags() const;

    [[nodiscard]] const char *get_tags_ptr() const;

private:
    const char *tags_ptr = nullptr;
    size_t tag_size{};
    uint32_t flags{};
};

#endif //FS_READER_JOURNAL_DESCRIPTOR_BLOCK_H
