//
// Created by andrew on 11/30/23.
//

#ifndef FS_READER_INODE_ITERATOR_H
#define FS_READER_INODE_ITERATOR_H

#include "filesystem/structs/inode.h"
#include "filesystem/types.h"


class InodeIterator {
    using value_type = block_t;
    using pointer = block_t *;
    using reference = block_t &;

public:
    InodeIterator() = default;

    explicit InodeIterator(const Inode *inode);

    InodeIterator &operator++();

    InodeIterator &operator+=(inode_block_t n);

    bool operator==(const InodeIterator &other) const;

    bool operator!=(const InodeIterator &other) const;

    value_type operator*() const;

    [[nodiscard]] Inode *get_inode();

private:
    const Inode *inode = nullptr;
    block_t block = 0;
    inode_block_t block_idx = 0;
};

#endif //FS_READER_INODE_ITERATOR_H
