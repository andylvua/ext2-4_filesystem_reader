//
// Created by andrew on 11/30/23.
//

#include "filesystem/structs/descriptor_table.h"

#include <ext2fs/ext2fs.h> // SUPERBLOCK_OFFSET, SUPERBLOCK_SIZE


DescriptorTable::DescriptorTable(const FilesystemDump &dump, const Superblock &superblock) {
    constexpr auto superblock_end = static_cast<block_size_t>(SUPERBLOCK_OFFSET + SUPERBLOCK_SIZE);
    int descriptor_table_offset = std::max(superblock_end, superblock.get_block_size());

    group_desc_table = dump.data<ext2_group_desc>(descriptor_table_offset);
    if (superblock->s_feature_incompat & EXT4_FEATURE_INCOMPAT_64BIT) {
        desc_size = superblock->s_desc_size;
    }
}

ext2_group_desc *DescriptorTable::get_group_desc(const uint32_t group_num) const {
    auto const group_desc_p = reinterpret_cast<char *>(group_desc_table) + group_num * desc_size;
    return reinterpret_cast<ext2_group_desc *>(group_desc_p);
}
