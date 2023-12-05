//
// Created by andrew on 11/24/23.
//

#include "mappings/flag_mappings.h"
#include "filesystem/journal/jbd2.h"

#include <ext2fs/ext2_fs.h>


const flags_map_decltype state_map = {
        {EXT2_VALID_FS,  "Valid FS"},
        {EXT2_ERROR_FS,  "Error FS"},
        {EXT3_ORPHAN_FS, "Orphan FS"},
        {EXT4_FC_REPLAY, "Fast commit replay"},
};

const flags_map_decltype err_behavior_map = {
        {EXT2_ERRORS_CONTINUE, "Continue"},
        {EXT2_ERRORS_RO,       "Remount read-only"},
        {EXT2_ERRORS_PANIC,    "Kernel panic"}
};

const flags_map_decltype os_map = {
        {EXT2_OS_LINUX,      "Linux"},
        {EXT2_OS_HURD,       "Hurd"},
        {EXT2_OBSO_OS_MASIX, "Masix"},
        {EXT2_OS_FREEBSD,    "FreeBSD"},
        {EXT2_OS_LITES,      "Lites"}
};

const flags_map_decltype compat_feature_map = {
        {EXT2_FEATURE_COMPAT_DIR_PREALLOC,   "Directory preallocation"},
        {EXT2_FEATURE_COMPAT_IMAGIC_INODES,  "Imagic inodes"},
        {EXT2_FEATURE_COMPAT_EXT_ATTR,       "Extended attributes"},
        {EXT2_FEATURE_COMPAT_RESIZE_INODE,   "Resize inode"},
        {EXT2_FEATURE_COMPAT_DIR_INDEX,      "Directory index"},
        {EXT2_FEATURE_COMPAT_LAZY_BG,        "Lazy block groups"},
        {EXT2_FEATURE_COMPAT_EXCLUDE_BITMAP, "Exclude bitmap"},
        {EXT4_FEATURE_COMPAT_SPARSE_SUPER2,  "Sparse superblock"},
        {EXT4_FEATURE_COMPAT_FAST_COMMIT,    "Fast commit"},
        {EXT4_FEATURE_COMPAT_STABLE_INODES,  "Stable inodes"},
        {EXT4_FEATURE_COMPAT_ORPHAN_FILE,    "Orphan file"},
};

const flags_map_decltype ro_compat_feature_map = {
        {EXT2_FEATURE_RO_COMPAT_SPARSE_SUPER,   "Sparse superblock"},
        {EXT2_FEATURE_RO_COMPAT_LARGE_FILE,     "Large file"},
        {EXT4_FEATURE_RO_COMPAT_HUGE_FILE,      "Huge file"},
        {EXT4_FEATURE_RO_COMPAT_GDT_CSUM,       "Group descriptor checksum"},
        {EXT4_FEATURE_RO_COMPAT_DIR_NLINK,      "Directory nlink"},
        {EXT4_FEATURE_RO_COMPAT_EXTRA_ISIZE,    "Extra inode size"},
        {EXT4_FEATURE_RO_COMPAT_HAS_SNAPSHOT,   "Has snapshot"},
        {EXT4_FEATURE_RO_COMPAT_QUOTA,          "Quota"},
        {EXT4_FEATURE_RO_COMPAT_BIGALLOC,       "Bigalloc"},
        {EXT4_FEATURE_RO_COMPAT_METADATA_CSUM,  "Metadata checksum"},
        {EXT4_FEATURE_RO_COMPAT_REPLICA,        "Replica"},
        {EXT4_FEATURE_RO_COMPAT_READONLY,       "Read-only"},
        {EXT4_FEATURE_RO_COMPAT_PROJECT,        "Project quota"},
        {EXT4_FEATURE_RO_COMPAT_SHARED_BLOCKS,  "Shared blocks"},
        {EXT4_FEATURE_RO_COMPAT_VERITY,         "Verity"},
        {EXT4_FEATURE_RO_COMPAT_ORPHAN_PRESENT, "Orphan present"},
};

const flags_map_decltype incompat_feature_map = {
        {EXT2_FEATURE_INCOMPAT_COMPRESSION, "Compression"},
        {EXT2_FEATURE_INCOMPAT_FILETYPE,    "Filetype"},
        {EXT2_FEATURE_INCOMPAT_META_BG,     "Meta block group"},
        {EXT3_FEATURE_INCOMPAT_RECOVER,     "Needs recovery"},
        {EXT3_FEATURE_INCOMPAT_JOURNAL_DEV, "Journal device"},
        {EXT3_FEATURE_INCOMPAT_EXTENTS,     "Extents"},
        {EXT4_FEATURE_INCOMPAT_64BIT,       "64-bit"},
        {EXT4_FEATURE_INCOMPAT_MMP,         "Multiple mount protection"},
        {EXT4_FEATURE_INCOMPAT_FLEX_BG,     "Flexible block groups"},
        {EXT4_FEATURE_INCOMPAT_EA_INODE,    "EA inode"},
        {EXT4_FEATURE_INCOMPAT_DIRDATA,     "Directory data"},
        {EXT4_FEATURE_INCOMPAT_CSUM_SEED,   "Checksum seed"},
        {EXT4_FEATURE_INCOMPAT_LARGEDIR,    "Large directory"},
        {EXT4_FEATURE_INCOMPAT_INLINE_DATA, "Inline data"},
        {EXT4_FEATURE_INCOMPAT_ENCRYPT,     "Encryption"},
        {EXT4_FEATURE_INCOMPAT_CASEFOLD,    "Casefold"},
};

const flags_map_decltype misc_flags_map = {
        {EXT2_FLAGS_SIGNED_HASH,   "Signed dirhash in use"},
        {EXT2_FLAGS_UNSIGNED_HASH, "Unsigned dirhash in use"},
        {EXT2_FLAGS_TEST_FILESYS,  "OK for use on development code"},
        {EXT2_FLAGS_IS_SNAPSHOT,   "This is a snapshot image"},
        {EXT2_FLAGS_FIX_SNAPSHOT,  "Snapshot inodes corrupted"},
        {EXT2_FLAGS_FIX_EXCLUDE,   "Exclude bitmaps corrupted"},
};

const flags_map_decltype default_mount_opts_map = {
        {EXT2_DEFM_DEBUG,      "Print debugging info upon (re)mount"},
        {EXT2_DEFM_BSDGROUPS,  "BSD groups enabled"},
        {EXT2_DEFM_XATTR_USER, "Extended user attributes enabled"},
        {EXT2_DEFM_ACL,        "ACLs enabled"},
        {EXT2_DEFM_UID16,      "Use 16-bit UIDs"},
        {EXT3_DEFM_JMODE,      "Journaling mode"},
        {EXT4_DEFM_NOBARRIER,  "Disable write barriers"},
        {EXT4_DEFM_BLOCK_VALIDITY, "Enable block validity checking"},
        {EXT4_DEFM_DISCARD,    "Enable discard"},
        {EXT4_DEFM_NODELALLOC, "Disable delayed allocation"},
};

const flags_map_decltype journal_mode_map = {
        {EXT3_DEFM_JMODE_DATA,    "data=journal"},
        {EXT3_DEFM_JMODE_ORDERED, "data=ordered"},
        {EXT3_DEFM_JMODE_WBACK,   "data=writeback"},
};

const flags_map_decltype file_type_map = {
        {EXT2_FT_UNKNOWN,  "UNKN"},
        {EXT2_FT_REG_FILE, "REGF"},
        {EXT2_FT_DIR,      "DIR"},
        {EXT2_FT_CHRDEV,   "CDEV"},
        {EXT2_FT_BLKDEV,   "BDEV"},
        {EXT2_FT_FIFO,     "FIFO"},
        {EXT2_FT_SOCK,     "SOCK"},
        {EXT2_FT_SYMLINK,  "SYML"},
};

const flags_map_decltype inode_flags_map = {
        {EXT2_SECRM_FL,     "SEC_RM"},
        {EXT2_UNRM_FL,      "UNRM"},
        {EXT2_COMPR_FL,     "COMPR"},
        {EXT2_SYNC_FL,      "SYNC"},
        {EXT2_IMMUTABLE_FL, "IMMUT"},
        {EXT2_APPEND_FL,    "APPEND"},
        {EXT2_NODUMP_FL,    "NODUMP"},
        {EXT2_NOATIME_FL,   "NOATIM"},
        {EXT2_DIRTY_FL,     "DIRTY"},
        {EXT2_COMPRBLK_FL,  "COMPRB"},
        {EXT2_NOCOMPR_FL,   "NOCOMP"},
        {EXT2_BTREE_FL,     "BTREE"},
        {EXT2_INDEX_FL,     "INDEX"},
        {EXT2_IMAGIC_FL,    "IMAGIC"},
        {EXT2_NOTAIL_FL,    "NOTAIL"},
        {EXT2_DIRSYNC_FL,   "DIRSYN"},
        {EXT2_TOPDIR_FL,    "TOPDIR"},
        {FS_NOCOW_FL,       "NOCOW"},
        {FS_DAX_FL,         "DAX"},
        {EXT2_RESERVED_FL,  "RESERV"},
        {EXT4_ENCRYPT_FL,   "ENCRYPT"},
        {EXT3_JOURNAL_DATA_FL, "JOURNAL_DATA"},
        {EXT4_EXTENTS_FL,   "EXTNT"},
        {EXT4_HUGE_FILE_FL, "HUGE_FILE"},
        {EXT4_VERITY_FL,    "VERITY"},
        {EXT4_EA_INODE_FL,  "EA_INODE"},
        {EXT4_SNAPFILE_FL,  "SNAPFILE"},
        {EXT4_SNAPFILE_DELETED_FL, "SNAPFILE_DELETED"},
        {EXT4_SNAPFILE_SHRUNK_FL, "SNAPFILE_SHRUNK"},
        {EXT4_INLINE_DATA_FL, "INL_DATA"},
        {EXT4_PROJINHERIT_FL, "PROJINHERIT"},
        {EXT4_CASEFOLD_FL,  "CASEFOLD"},
};

const flags_map_decltype partition_types_map = {
        {0x00, "Empty"},
        {0x01, "FAT12"},
        {0x04, "FAT16 <32M"},
        {0x05, "Extended"},
        {0x06, "FAT16"},
        {0x07, "HPFS/NTFS/exFAT"},
        {0x08, "AIX"},
        {0x09, "AIX bootable"},
        {0x0a, "OS/2 Boot Manager"},
        {0x0b, "W95 FAT32"},
        {0x0c, "W95 FAT32 (LBA)"},
        {0x0e, "W95 FAT16 (LBA)"},
        {0x0f, "W95 Ext'd (LBA)"},
        {0x10, "OPUS"},
        {0x11, "Hidden FAT12"},
        {0x12, "Compaq diagnostics"},
        {0x14, "Hidden FAT16 <32M"},
        {0x16, "Hidden FAT16"},
        {0x17, "Hidden HPFS/NTFS"},
        {0x18, "AST SmartSleep"},
        {0x1b, "Hidden W95 FAT32"},
        {0x1c, "Hidden W95 FAT32 (LBA)"},
        {0x1e, "Hidden W95 FAT16 (LBA)"},
        {0x24, "NEC DOS"},
        {0x27, "Hidden NTFS WinRE"},
        {0x39, "Plan 9"},
        {0x3c, "PartitionMagic recovery"},
        {0x40, "Venix 80286"},
        {0x41, "PPC PReP Boot"},
        {0x42, "SFS"},
        {0x4d, "QNX4.x"},
        {0x4e, "QNX4.x 2nd part"},
        {0x4f, "QNX4.x 3rd part"},
        {0x50, "OnTrack DM"},
        {0x51, "OnTrack DM6 Aux1"},
        {0x52, "CP/M"},
        {0x53, "OnTrack DM6 Aux3"},
        {0x54, "OnTrackDM6"},
        {0x55, "EZ-Drive"},
        {0x56, "Golden Bow"},
        {0x5c, "Priam Edisk"},
        {0x61, "SpeedStor"},
        {0x63, "GNU HURD or SysV"},
        {0x64, "Novell Netware 286"},
        {0x65, "Novell Netware 386"},
        {0x70, "DiskSecure Multi-Boot"},
        {0x75, "PC/IX"},
        {0x80, "Old Minix"},
        {0x81, "Minix / old Linux"},
        {0x82, "Linux swap / Solaris"},
        {0x83, "Linux"},
        {0x84, "OS/2 hidden C: drive"},
        {0x85, "Linux extended"},
        {0x86, "NTFS volume set"},
        {0x87, "NTFS volume set"},
        {0x88, "Linux plaintext"},
        {0x8e, "Linux LVM"},
        {0x93, "Amoeba"},
        {0x94, "Amoeba BBT"},
        {0x9f, "BSD/OS"},
        {0xa0, "IBM Thinkpad hibernation"},
        {0xa5, "FreeBSD"},
        {0xa6, "OpenBSD"},
        {0xa7, "NeXTSTEP"},
        {0xa8, "Darwin UFS"},
        {0xa9, "NetBSD"},
        {0xab, "Darwin boot"},
        {0xaf, "HFS / HFS+"},
        {0xb7, "BSDI fs"},
        {0xb8, "BSDI swap"},
        {0xbb, "Boot Wizard hidden"},
        {0xbe, "Solaris boot"},
        {0xbf, "Solaris"},
        {0xc1, "DRDOS/sec (FAT-12)"},
        {0xc4, "DRDOS/sec (FAT-16 < 32M)"},
        {0xc6, "DRDOS/sec (FAT-16)"},
        {0xc7, "Syrinx"},
        {0xda, "Non-FS data"},
        {0xdb, "CP/M / CTOS /"},
        {0xde, "Dell Utility"},
        {0xdf, "BootIt"},
        {0xe1, "DOS access"},
        {0xe3, "DOS R/O"},
        {0xe4, "SpeedStor"},
        {0xeb, "BeOS fs"},
        {0xee, "GPT"},
        {0xef, "EFI (FAT-12/16/32)"},
        {0xf0, "Linux/PA-RISC boot"},
        {0xf1, "SpeedStor"},
        {0xf2, "DOS secondary"},
        {0xf4, "SpeedStor"},
        {0xf5, "Prologue"},
        {0xf6, "SpeedStor"},
        {0xf7, "SpeedStor"},
        {0xf9, "pCache"},
        {0xfb, "VMware VMFS"},
        {0xfc, "VMware VMKCORE"},
        {0xfd, "Linux raid auto"},
        {0xfe, "LANstep"},
        {0xff, "BBT"},
};

const flags_map_decltype jbd2_compat_feature_map = {
        {JBD2_FEATURE_COMPAT_CHECKSUM, "Checksum"},
};

const flags_map_decltype jbd2_incompat_feature_map = {
        {JBD2_FEATURE_INCOMPAT_REVOKE,       "Revoke"},
        {JBD2_FEATURE_INCOMPAT_64BIT,        "64-bit"},
        {JBD2_FEATURE_INCOMPAT_ASYNC_COMMIT, "Async commit"},
        {JBD2_FEATURE_INCOMPAT_CSUM_V2,      "Checksum v2"},
        {JBD2_FEATURE_INCOMPAT_CSUM_V3,      "Checksum v3"},
        {JBD2_FEATURE_INCOMPAT_FAST_COMMIT,  "Fast commit"},
};

const flags_map_decltype jbd2_ro_compat_feature_map = {};
