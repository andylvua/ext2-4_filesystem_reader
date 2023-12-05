// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
//
// Created by andrew on 12/1/23.
//

#include "filesystem/journal/structs/iterators/descriptor_block_iterator.h"


DescBlockIterator::DescBlockIterator(const JournalDescriptorBlock *desc_block) : desc_block(desc_block) {
    if (desc_block == nullptr) {
        block_tag = DescriptorTag();
        return;
    }
    block_tag = DescriptorTag(desc_block->get_tags_ptr(), desc_block->get_flags());
}

DescBlockIterator &DescBlockIterator::operator++() {
    if (block_tag.get_flags() & JBD2_FLAG_LAST_TAG) {
        block_tag = DescriptorTag();
        return *this;
    }

    offset += desc_block->get_tag_size();
    if (!(block_tag.get_flags() & JBD2_FLAG_SAME_UUID)) {
        offset += sizeof(char[16]);
    }
    block_tag = DescriptorTag(desc_block->get_tags_ptr() + offset, desc_block->get_flags());
    return *this;
}

bool DescBlockIterator::operator==(const DescBlockIterator &other) const {
    return block_tag == other.block_tag;
}

bool DescBlockIterator::operator!=(const DescBlockIterator &other) const {
    return !(*this == other);
}

DescBlockIterator::reference DescBlockIterator::operator*() {
    return block_tag;
}
