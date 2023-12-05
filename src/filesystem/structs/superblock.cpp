// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
//
// Created by andrew on 11/30/23.
//

#include "filesystem/structs/superblock.h"
#include "errors/exception.h"
#include "filesystem/types.h"

#include <ext2fs/ext2fs.h> // SUPERBLOCK_OFFSET
#include <iomanip>


Superblock::Superblock(const FilesystemDump &dump) {
    auto *superblock = dump.data<ext2_super_block>(SUPERBLOCK_OFFSET);

    if (superblock->s_magic != EXT2_SUPER_MAGIC) {
        throw FSReaderException("Bad superblock: not an extX filesystem");
    }

    std::cout << "Superblock: OK\n";
    data = superblock;
}

void Superblock::print() const {
    std::stringstream superblock_info;

    superblock_info << "Filesystem volume name: " << data->s_volume_name << std::endl;
    superblock_info << "Last mounted on: " << data->s_last_mounted << std::endl;
    superblock_info << "Filesystem UUID: " << format_uuid(data->s_uuid) << std::endl;
    superblock_info << "Filesystem magic number: 0x" << std::hex << std::uppercase
    << data->s_magic << std::dec << std::endl;
    superblock_info << "Filesystem revision #: " << data->s_rev_level << std::endl;
    superblock_info << "Filesystem features: ";
    superblock_info << format_flags(compat_feature_map, data->s_feature_compat);
    superblock_info << format_flags(ro_compat_feature_map, data->s_feature_ro_compat);
    superblock_info << format_flags(incompat_feature_map, data->s_feature_incompat) << std::endl;
    superblock_info << "Filesystem flags: ";
    superblock_info << format_flags(misc_flags_map, data->s_flags) << std::endl;
    superblock_info << "Default mount options: ";
    superblock_info << format_flags(default_mount_opts_map, data->s_default_mount_opts) << std::endl;
    superblock_info << "Filesystem state: ";
    superblock_info << format_flags(state_map, data->s_state) << std::endl;
    superblock_info << "Errors behavior: ";
    superblock_info << format_flags(err_behavior_map, data->s_errors, false) << std::endl;
    superblock_info << "Filesystem OS type: ";
    superblock_info << format_flags(os_map, data->s_creator_os, false) << std::endl;
    superblock_info << "Inode count: " << data->s_inodes_count << std::endl;
    auto block_count = static_cast<block_t>(data->s_blocks_count_hi) << 32 | data->s_blocks_count;
    superblock_info << "Block count: " << block_count << std::endl;
    auto reserved_block_count = static_cast<block_t>(data->s_r_blocks_count_hi) << 32 | data->s_r_blocks_count;
    superblock_info << "Reserved block count: " << reserved_block_count << std::endl;
    superblock_info << "Overhead clusters: " << data->s_overhead_clusters << std::endl;
    auto free_block_count = static_cast<block_t>(data->s_free_blocks_hi) << 32 | data->s_free_blocks_count;
    superblock_info << "Free blocks: " << free_block_count << std::endl;
    superblock_info << "Free inodes: " << data->s_free_inodes_count << std::endl;
    superblock_info << "First data block: " << data->s_first_data_block << std::endl;
    superblock_info << "Block size: " << (1024 << data->s_log_block_size) << std::endl;
    superblock_info << "Reserved GDT blocks: " << data->s_reserved_gdt_blocks << std::endl;
    superblock_info << "Blocks per group: " << data->s_blocks_per_group << std::endl;
    superblock_info << "Inodes per group: " << data->s_inodes_per_group << std::endl;
    superblock_info << "Inode blocks per group: " << data->s_inode_size << std::endl;
    auto creation_time = static_cast<uint64_t>(data->s_mkfs_time_hi) << 32 | data->s_mkfs_time;
    superblock_info << "Filesystem created: " << format_time(creation_time) << std::endl;
    auto last_mount_time = static_cast<uint64_t>(data->s_mtime_hi) << 32 | data->s_mtime;
    superblock_info << "Last mount time: " << format_time(last_mount_time) << std::endl;
    auto write_time = static_cast<uint64_t>(data->s_wtime_hi) << 32 | data->s_wtime;
    superblock_info << "Last write time: " << format_time(write_time) << std::endl;
    superblock_info << "Mount count: " << data->s_mnt_count << std::endl;
    superblock_info << "Maximal mount count: " << data->s_max_mnt_count << std::endl;
    auto last_check_time = static_cast<uint64_t>(data->s_lastcheck_hi) << 32 | data->s_lastcheck;
    superblock_info << "Last checked: " << format_time(last_check_time) << std::endl;
    superblock_info << "Check interval: " << data->s_checkinterval << std::endl;
    superblock_info << "Lifetime writes: " << data->s_kbytes_written << std::endl;
    superblock_info << "Reserved blocks uid: " << data->s_def_resuid << std::endl;
    superblock_info << "Reserved blocks gid: " << data->s_def_resgid << std::endl;
    superblock_info << "First inode: " << data->s_first_ino << std::endl;
    superblock_info << "Inode size: " << data->s_inode_size << std::endl;

    std::cout << superblock_info.str();
}

ext2_super_block* Superblock::operator->() const {
    return data;
}

block_size_t Superblock::get_block_size() const {
    return 1024 << data->s_log_block_size;
}
