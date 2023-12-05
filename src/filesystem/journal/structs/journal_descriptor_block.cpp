// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
//
// Created by andrew on 11/30/23.
//
#include "filesystem/journal/structs/journal_descriptor_block.h"
#include "filesystem/journal/structs/iterators/descriptor_block_iterator.h"


namespace {
    /* Source: https://github.com/torvalds/linux/blob/master/fs/jbd2/journal.c
     *
     * Copyright 1998 Red Hat corp --- All Rights Reserved
     * Written by Stephen C. Tweedie <sct@redhat.com>, 1998 */
    size_t journal_tag_bytes(uint32_t feature_incompat) {
        size_t sz;

        if (feature_incompat & JBD2_FEATURE_INCOMPAT_CSUM_V3)
            return sizeof(journal_block_tag3_t);

        sz = sizeof(journal_block_tag_t);
        if (feature_incompat & JBD2_FEATURE_INCOMPAT_CSUM_V2)
            sz += sizeof(uint16_t);

        if (feature_incompat & JBD2_FEATURE_INCOMPAT_64BIT)
            return sz;
        else
            return sz - sizeof(uint32_t);
    }
    /* End of source */
}


JournalDescriptorBlock::JournalDescriptorBlock(const journal_header_s *header, uint32_t feature_incompat)
        : tag_size(journal_tag_bytes(feature_incompat)), flags(feature_incompat) {
    tags_ptr = reinterpret_cast<const char *>(++header);
}

JournalDescriptorBlock::iterator JournalDescriptorBlock::begin() const {
    return iterator(this);
}

JournalDescriptorBlock::iterator JournalDescriptorBlock::end() const {
    return {};
}

void JournalDescriptorBlock::print() {
    for (const auto &block_tag: *this) {
        block_tag.print();
    }
}

uint32_t JournalDescriptorBlock::get_tag_size() const {
    return tag_size;
}

uint32_t JournalDescriptorBlock::get_flags() const {
    return flags;
}

const char *JournalDescriptorBlock::get_tags_ptr() const {
    return tags_ptr;
}
