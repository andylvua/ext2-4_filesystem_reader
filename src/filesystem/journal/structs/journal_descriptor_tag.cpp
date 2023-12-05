//
// Created by andrew on 11/30/23.
//

#include "filesystem/journal/structs/journal_descriptor_tag.h"
#include "utils/utils.h"

#include <iostream>


DescriptorTag::DescriptorTag(const char *tag, uint32_t incompat_flags) : incompat_flags(incompat_flags) {
    if (incompat_flags & JBD2_FEATURE_INCOMPAT_CSUM_V3) {
        this->tag.tag3 = reinterpret_cast<const journal_block_tag3_s *>(tag);
    } else {
        this->tag.tag = reinterpret_cast<const journal_block_tag_s *>(tag);
    }
}

block_t DescriptorTag::get_block_num() const {
    if (incompat_flags & JBD2_FEATURE_INCOMPAT_CSUM_V3) {
        return swap_endian(tag.tag3->t_blocknr) |
               static_cast<uint64_t>(swap_endian(tag.tag3->t_blocknr_high)) << 32;
    }

    if (incompat_flags & JBD2_FEATURE_INCOMPAT_64BIT) {
        return swap_endian(tag.tag->t_blocknr) |
               static_cast<uint64_t>(swap_endian(tag.tag->t_blocknr_high)) << 32;
    } else {
        return swap_endian(tag.tag->t_blocknr);
    }
}

uint32_t DescriptorTag::get_flags() const {
    if (incompat_flags & JBD2_FEATURE_INCOMPAT_CSUM_V3) {
        return swap_endian(tag.tag3->t_flags);
    }

    return swap_endian(tag.tag->t_flags);
}

void DescriptorTag::print() const {
    auto flags = get_flags();

    if (flags & JBD2_FLAG_SAME_UUID) {
        std::cout << "Same UUID" << std::endl;
    }
    if (flags & JBD2_FLAG_DELETED) {
        std::cout << "Deleted" << std::endl;
        return;
    }
    if (flags & JBD2_FLAG_ESCAPE) {
        std::cout << "Escaped" << std::endl;
        return;
    }

    std::cout << "Block number: " << get_block_num() << std::endl;
    std::cout << "Block flags: " << get_flags() << std::endl;

    if (flags & JBD2_FLAG_LAST_TAG) {
        std::cout << "Last tag" << std::endl;
    }
}

bool operator==(const DescriptorTag &lhs, const DescriptorTag &rhs) {
    if (lhs.incompat_flags & JBD2_FEATURE_INCOMPAT_CSUM_V3) {
        return lhs.tag.tag3 == rhs.tag.tag3;
    } else {
        return lhs.tag.tag == rhs.tag.tag;
    }
}
