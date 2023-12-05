//
// Created by andrew on 11/29/23.
//

#include "filesystem/structs/inode.h"
#include "filesystem/structs/iterators/inode_iterator.h"
#include "filesystem/constants.h"

#include <ext2fs/ext2_fs.h>
#include <ext2fs/ext2_ext_attr.h>
#include <sys/stat.h>


Inode::Inode(const FSInfo *fs_info, const uint32_t inode_num) : fs_info(fs_info) {
    if (inode_num == 0 || inode_num > fs_info->super_block->s_inodes_count) {
        return;
    }
    auto const group = (inode_num - 1) / fs_info->super_block->s_inodes_per_group;
    auto const ingroup_index = (inode_num - 1) % fs_info->super_block->s_inodes_per_group;
    auto const ingroup_block = (ingroup_index * fs_info->super_block->s_inode_size) / fs_info->block_size;
    auto const inblock_offset = (ingroup_index * fs_info->super_block->s_inode_size) % fs_info->block_size;

    auto const *group_desc = fs_info->descriptor_table.get_group_desc(group);
    block = (group_desc->bg_inode_table + ingroup_block);
    auto const offset = block * fs_info->block_size + inblock_offset;
    inode = fs_info->get_data<ext2_inode>(offset);

    read_inline_data();
}

Inode::iterator Inode::begin() const {
    return InodeIterator(this);
}

Inode::iterator Inode::end() const {
    return {};
}

ext2_inode *Inode::operator->() const {
    return inode;
}

block_t Inode::get_block(inode_block_t block_num) const {
    if (inode == nullptr) {
        return 0;
    }
    // If the inode is a symbolic link or a regular file with inline data, data
    // is stored in the inode itself (only up to 60 bytes).
    if (inode->i_flags & EXT4_INLINE_DATA_FL || (inode->i_mode & S_IFLNK && inode->i_size < IBLOCK_SIZE)) {
        return block;
    }
    if (is_extent()) {
        return extent_to_block(reinterpret_cast<ext3_extent_header *>(inode->i_block), block_num);
    }
    return logical_to_block(block_num);
}

size_t Inode::get_size() const {
    if (inode == nullptr) {
        return 0;
    }
    size_t size = inode->i_size;
    if (inode->i_flags & EXT4_HUGE_FILE_FL) {
        size |= static_cast<size_t>(inode->i_size_high) << 32;
    }
    return size;
}

bool Inode::is_extent() const {
    return inode->i_flags & EXT4_EXTENTS_FL;
}

bool Inode::has_inline_data() const {
    return inode->i_flags & EXT4_INLINE_DATA_FL;
}

bool Inode::is_valid() const {
    return inode != nullptr;
}

void Inode::read_inline_data() {
    if (!has_inline_data()) {
        return;
    }

    auto *data = reinterpret_cast<char *>(inode->i_block);
    if (inode->i_size < IBLOCK_SIZE) {
        inline_data.assign(data, data + inode->i_size);
        return;
    }

    inline_data.reserve(inode->i_size);
    inline_data.insert(inline_data.end(), data, data + IBLOCK_SIZE);

    /* The presence of inline data ensure that this is ext4 inode (ext2_inode_large)
     * and system.data EA is present. */
    auto *l_inode = reinterpret_cast<ext2_inode_large *>(inode);

    auto xattr_header_p = reinterpret_cast<char *>(inode + 1) + l_inode->i_extra_isize;
    auto xattr_header = reinterpret_cast<uint32_t *>(xattr_header_p);

    auto xattr_entry_p = xattr_header + 1;
    auto xattr_entry = reinterpret_cast<ext2_ext_attr_entry *>(xattr_entry_p);

    while (reinterpret_cast<char *>(xattr_entry) <= reinterpret_cast<char *>(inode) +
                                                    fs_info->super_block->s_inode_size -
                                                    sizeof(ext2_ext_attr_entry)) {

        if (xattr_entry->e_name_index == XATTR_DATA_IDX) {
            break;
        }
        xattr_entry = reinterpret_cast<ext2_ext_attr_entry *>(reinterpret_cast<char *>(xattr_entry + 1) +
                                                              xattr_entry->e_name_len);
    }

    auto data_p = reinterpret_cast<char *>(xattr_entry) + xattr_entry->e_value_offs;
    inline_data.insert(inline_data.end(), data_p, data_p + xattr_entry->e_value_size);
}

char* Inode::get_inline_data() {
    if (!has_inline_data()) {
        return {};
    }

    return inline_data.data();
}

block_t Inode::extent_to_block(ext3_extent_header *ex_header, inode_block_t block_num) const { // NOLINT
    if (ex_header == nullptr) {
        return 0;
    }

    auto entries = ex_header->eh_entries;

    if (ex_header->eh_depth == 0) {
        auto *extent = reinterpret_cast<ext3_extent *>(ex_header + 1);
        for (int i = 0; i < entries; ++i) {
            if (block_num >= extent->ee_block &&
                block_num < extent->ee_block + extent->ee_len) {
                return (extent->ee_start | static_cast<block_t>(extent->ee_start_hi) << 32) +
                       block_num - extent->ee_block;
            }
            ++extent;
        }
        return 0;
    }

    auto *index = reinterpret_cast<ext3_extent_idx *>(ex_header + 1);
    for (int i = 0; i < entries; ++i) {
        if (i == entries - 1 || block_num < (index + 1)->ei_block) {
            auto const next_block = index->ei_leaf | static_cast<block_t>(index->ei_leaf_hi) << 32;
            return extent_to_block(fs_info->get_block<ext3_extent_header>(next_block), block_num);
        }
        ++index;
    }
    return 0;
}

block_t Inode::logical_to_block(inode_block_t block_num) const {
    auto const blocks = reinterpret_cast<uint32_t *>(inode->i_block);
    if (block_num < EXT2_NDIR_BLOCKS) {
        return blocks[block_num];
    }

    inode_block_t ind_block_num = fs_info->block_size / sizeof(uint32_t);
    if (block_num < EXT2_NDIR_BLOCKS + ind_block_num) {
        if (blocks[EXT2_IND_BLOCK] == 0) {
            return 0;
        }
        auto const indirect_block = fs_info->get_block<uint32_t>(blocks[EXT2_IND_BLOCK]);
        return indirect_block[block_num - EXT2_NDIR_BLOCKS];
    }

    inode_block_t dind_block_num = ind_block_num * ind_block_num;
    if (block_num < EXT2_NDIR_BLOCKS + ind_block_num + dind_block_num) {
        if (blocks[EXT2_DIND_BLOCK] == 0) {
            return 0;
        }
        auto const doubly_indirect_block = fs_info->get_block<uint32_t>(blocks[EXT2_DIND_BLOCK]);
        auto const indirect_block = fs_info->get_block<uint32_t>(
                doubly_indirect_block[(block_num - EXT2_NDIR_BLOCKS - ind_block_num) / ind_block_num]);
        return indirect_block[(block_num - EXT2_NDIR_BLOCKS - ind_block_num) % ind_block_num];
    }

    inode_block_t tind_block_num = dind_block_num * ind_block_num;
    if (block_num < EXT2_NDIR_BLOCKS + ind_block_num + dind_block_num + tind_block_num) {
        if (blocks[EXT2_TIND_BLOCK] == 0) {
            return 0;
        }
        auto const triply_indirect_block = fs_info->get_block<uint32_t>(blocks[EXT2_TIND_BLOCK]);
        auto const doubly_indirect_block = fs_info->get_block<uint32_t>(
                triply_indirect_block[(block_num - EXT2_NDIR_BLOCKS - ind_block_num - dind_block_num) /
                                      dind_block_num]);
        auto const indirect_block = fs_info->get_block<uint32_t>(
                doubly_indirect_block[(block_num - EXT2_NDIR_BLOCKS - ind_block_num - dind_block_num) %
                                      dind_block_num]);
        return indirect_block[(block_num - EXT2_NDIR_BLOCKS - ind_block_num - dind_block_num) % dind_block_num];
    }

    return 0;
}

bool operator==(const Inode &lhs, const Inode &rhs) {
    return lhs.inode == rhs.inode;
}
