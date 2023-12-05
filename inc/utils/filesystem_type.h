//
// Created by andrew on 11/26/23.
//

#ifndef FS_READER_FS_TYPE_H
#define FS_READER_FS_TYPE_H

#include "filesystem/structs/superblock.h"


/* Source: https://git.kernel.org/pub/scm/utils/util-linux/util-linux.git/tree/libblkid/src/superblocks/ext.c
 *
 * Copyright (C) 1999, 2001 by Andries Brouwer
 * Copyright (C) 1999, 2000, 2003 by Theodore Ts'o
 * Copyright (C) 2008 Karel Zak <kzak@redhat.com>
 */
#define EXT2_FEATURE_INCOMPAT_UNSUPP    ~(EXT2_FEATURE_INCOMPAT_FILETYPE| \
                                          EXT4_FEATURE_INCOMPAT_MMP| \
                                          EXT4_FEATURE_INCOMPAT_LARGEDIR| \
                                          EXT4_FEATURE_INCOMPAT_EA_INODE)
#define EXT2_FEATURE_RO_COMPAT_UNSUPP   ~(EXT2_FEATURE_RO_COMPAT_SPARSE_SUPER| \
                                          EXT2_FEATURE_RO_COMPAT_LARGE_FILE| \
                                          EXT4_FEATURE_RO_COMPAT_DIR_NLINK| \
                                          EXT4_FEATURE_RO_COMPAT_VERITY)

#define EXT3_FEATURE_RO_COMPAT_UNSUPP   ~(EXT2_FEATURE_RO_COMPAT_SPARSE_SUPER| \
                                          EXT2_FEATURE_RO_COMPAT_LARGE_FILE)
#define EXT3_FEATURE_INCOMPAT_UNSUPP    ~(EXT2_FEATURE_INCOMPAT_FILETYPE| \
                                          EXT3_FEATURE_INCOMPAT_RECOVER| \
                                          EXT2_FEATURE_INCOMPAT_META_BG)
/* End of source */

enum class FilesystemType {
    EXT2,
    EXT3,
    EXT4
};

inline FilesystemType get_fs_type(const Superblock& superblock) {
    using enum FilesystemType;
    auto type = EXT2;

    if (superblock->s_feature_compat & EXT3_FEATURE_COMPAT_HAS_JOURNAL) {
        type = EXT3;
    }

    if ((superblock->s_feature_ro_compat & EXT2_FEATURE_RO_COMPAT_UNSUPP) ||
        (superblock->s_feature_incompat & EXT2_FEATURE_INCOMPAT_UNSUPP)) {
        type = EXT3;
    }

    if (!(superblock->s_feature_compat & EXT3_FEATURE_COMPAT_HAS_JOURNAL) &&
        type == EXT3) {
        // wtf?
        type = EXT4;
    }

    if ((superblock->s_feature_ro_compat & EXT3_FEATURE_RO_COMPAT_UNSUPP) ||
        (superblock->s_feature_incompat & EXT3_FEATURE_INCOMPAT_UNSUPP)) {
        type = EXT4;
    }

    return type;
}

inline auto get_fs_type_name(const FilesystemType type) {
    switch (type) {
        case FilesystemType::EXT2:
            return "ext2";
        case FilesystemType::EXT3:
            return "ext3";
        case FilesystemType::EXT4:
            return "ext4";
        default: return "unknown"; // shut up compiler
    }
}

#endif //FS_READER_FS_TYPE_H
