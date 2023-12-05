//
// Created by andrew on 11/30/23.
//

#include "filesystem/structs/inode.h"
#include "filesystem/structs/iterators/inode_iterator.h"


InodeIterator::InodeIterator(const Inode *inode) : inode(inode) {
    if (inode == nullptr) {
        return;
    }
    this->block = inode->get_block(block_idx);
}

InodeIterator &InodeIterator::operator++() {
    if (inode == nullptr) {
        return *this;
    }

    ++block_idx;
    block = inode->get_block(block_idx);

    if (inode->is_extent()) {
        return *this;
    }

    inode_block_t ind_block_num = inode->fs_info->block_size / sizeof(uint32_t);
    inode_block_t dind_block_num = ind_block_num * ind_block_num;
    inode_block_t tind_block_num = dind_block_num * ind_block_num;

    while (block == 0 && block_idx < EXT2_NDIR_BLOCKS + ind_block_num + dind_block_num + tind_block_num) {
        ++block_idx;
        auto const blocks = inode->operator->()->i_block;
        if (EXT2_NDIR_BLOCKS <= block_idx && block_idx < EXT2_NDIR_BLOCKS + ind_block_num) {
            if (blocks[EXT2_IND_BLOCK] == 0) {
                block_idx = EXT2_NDIR_BLOCKS + ind_block_num;
            }
        }
        if (EXT2_NDIR_BLOCKS + ind_block_num <= block_idx &&
            block_idx < EXT2_NDIR_BLOCKS + ind_block_num + dind_block_num) {
            if (blocks[EXT2_DIND_BLOCK] == 0) {
                block_idx = EXT2_NDIR_BLOCKS + ind_block_num + dind_block_num;
            }
        }
        if (EXT2_NDIR_BLOCKS + ind_block_num + dind_block_num <= block_idx &&
            block_idx < EXT2_NDIR_BLOCKS + ind_block_num + dind_block_num + tind_block_num) {
            if (blocks[EXT2_TIND_BLOCK] == 0) {
                block_idx = EXT2_NDIR_BLOCKS + ind_block_num + dind_block_num + tind_block_num;
            }
        }

        block = inode->get_block(block_idx);
    }

    return *this;
}

InodeIterator &InodeIterator::operator+=(inode_block_t n) {
    block_idx += n - 1;
    return ++(*this);
}

bool InodeIterator::operator==(const InodeIterator &other) const {
    return this->block == other.block;
}

bool InodeIterator::operator!=(const InodeIterator &other) const {
    return !(*this == other);
}

InodeIterator::value_type InodeIterator::operator*() const {
    return block;
}

Inode *InodeIterator::get_inode() {
    return const_cast<Inode *>(inode);
}
