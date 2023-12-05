//
// Created by andrew on 11/30/23.
//

#ifndef FS_READER_DESCRIPTOR_BLOCK_ITERATOR_H
#define FS_READER_DESCRIPTOR_BLOCK_ITERATOR_H

#include "filesystem/journal/structs/journal_descriptor_block.h"
#include "filesystem/types.h"


class DescBlockIterator {
    using value_type = DescriptorTag;
    using reference = DescriptorTag &;
    using pointer = DescriptorTag *;

public:
    DescBlockIterator() = default;

    explicit DescBlockIterator(const JournalDescriptorBlock *desc_block);

    DescBlockIterator &operator++();

    bool operator==(const DescBlockIterator &other) const;

    bool operator!=(const DescBlockIterator &other) const;

    reference operator*();

private:
    const JournalDescriptorBlock *desc_block = nullptr;
    DescriptorTag block_tag{};
    block_size_t offset = 0;
};

#endif //FS_READER_DESCRIPTOR_BLOCK_ITERATOR_H
