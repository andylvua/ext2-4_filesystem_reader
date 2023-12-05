//
// Created by andrew on 11/25/23.
//

#ifndef FS_INFO_H
#define FS_INFO_H

#include "filesystem/journal/journal.h"
#include "filesystem_info.h"


struct Filesystem {
    FSInfo fs_info;
    Journal journal;
    bool has_journal = false;

    explicit Filesystem(const FilesystemDump &dump) : fs_info(dump) {
        if (fs_info.super_block->s_feature_compat & EXT3_FEATURE_COMPAT_HAS_JOURNAL) {
            journal = Journal(&fs_info);
            has_journal = true;
        }
    }
};

#endif //FS_INFO_H
