/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * linux/include/linux/jbd2.h
 *
 * Written by Stephen C. Tweedie <sct@redhat.com>
 *
 * Copyright 1998-2000 Red Hat, Inc --- All Rights Reserved
 *
 * Definitions for transaction data structures for the buffer cache
 * filesystem journaling support.
 */

/* _______NOTE_______
 * Andrii Yaroshevych
 *
 * This header is modified from the original version in the kernel
 * to remove unnecessary definitions.
 * Used only for structures and constants definitions.
 */

#ifndef LINUX_JBD2_H
#define LINUX_JBD2_H

#include <linux/types.h>
#include <linux/stddef.h>


#define JBD2_MAGIC_NUMBER 0xc03b3998U /* The first 4 bytes of /dev/random! */

/*
 * On-disk structures
 */

/*
 * Descriptor block types:
 */

#define JBD2_DESCRIPTOR_BLOCK	1
#define JBD2_COMMIT_BLOCK	2
#define JBD2_SUPERBLOCK_V1	3
#define JBD2_SUPERBLOCK_V2	4
#define JBD2_REVOKE_BLOCK	5

/*
 * Standard header for all descriptor blocks:
 */
typedef struct journal_header_s
{
	__be32		h_magic;
	__be32		h_blocktype;
	__be32		h_sequence;
} journal_header_t;

/*
 * Checksum types.
 */
#define JBD2_CRC32_CHKSUM   1
#define JBD2_MD5_CHKSUM     2
#define JBD2_SHA1_CHKSUM    3
#define JBD2_CRC32C_CHKSUM  4

#define JBD2_CRC32_CHKSUM_SIZE 4

#define JBD2_CHECKSUM_BYTES (32 / sizeof(__u32))
/*
 * Commit block header for storing transactional checksums:
 *
 * NOTE: If FEATURE_COMPAT_CHECKSUM (checksum v1) is set, the h_chksum*
 * fields are used to store a checksum of the descriptor and data blocks.
 *
 * If FEATURE_INCOMPAT_CSUM_V2 (checksum v2) is set, then the h_chksum
 * field is used to store crc32c(uuid+commit_block).  Each journal metadata
 * block gets its own checksum, and data block checksums are stored in
 * journal_block_tag (in the descriptor).  The other h_chksum* fields are
 * not used.
 *
 * If FEATURE_INCOMPAT_CSUM_V3 is set, the descriptor block uses
 * journal_block_tag3_t to store a full 32-bit checksum.  Everything else
 * is the same as v2.
 *
 * Checksum v1, v2, and v3 are mutually exclusive features.
 */
struct commit_header {
	__be32		h_magic;
	__be32          h_blocktype;
	__be32          h_sequence;
	unsigned char   h_chksum_type;
	unsigned char   h_chksum_size;
	unsigned char 	h_padding[2];
	__be32 		h_chksum[JBD2_CHECKSUM_BYTES];
	__be64		h_commit_sec;
	__be32		h_commit_nsec;
};

/*
 * The block tag: used to describe a single buffer in the journal.
 * t_blocknr_high is only used if INCOMPAT_64BIT is set, so this
 * raw struct shouldn't be used for pointer math or sizeof() - use
 * journal_tag_bytes(journal) instead to compute this.
 */
typedef struct journal_block_tag3_s
{
	__be32		t_blocknr;	/* The on-disk block number */
	__be32		t_flags;	/* See below */
	__be32		t_blocknr_high; /* most-significant high 32bits. */
	__be32		t_checksum;	/* crc32c(uuid+seq+block) */
} journal_block_tag3_t;

typedef struct journal_block_tag_s
{
	__be32		t_blocknr;	/* The on-disk block number */
	__be16		t_checksum;	/* truncated crc32c(uuid+seq+block) */
	__be16		t_flags;	/* See below */
	__be32		t_blocknr_high; /* most-significant high 32bits. */
} journal_block_tag_t;

/* Tail of descriptor or revoke block, for checksumming */
struct jbd2_journal_block_tail {
	__be32		t_checksum;	/* crc32c(uuid+descr_block) */
};

/*
 * The revoke descriptor: used on disk to describe a series of blocks to
 * be revoked from the log
 */
typedef struct jbd2_journal_revoke_header_s
{
	journal_header_t r_header;
	__be32		 r_count;	/* Count of bytes used in the block */
} jbd2_journal_revoke_header_t;

/* Definitions for the journal tag flags word: */
#define JBD2_FLAG_ESCAPE		1	/* on-disk block is escaped */
#define JBD2_FLAG_SAME_UUID	2	/* block has same uuid as previous */
#define JBD2_FLAG_DELETED	4	/* block deleted by this transaction */
#define JBD2_FLAG_LAST_TAG	8	/* last tag in this descriptor block */


/*
 * The journal superblock.  All fields are in big-endian byte order.
 */
typedef struct journal_superblock_s
{
/* 0x0000 */
	journal_header_t s_header;

/* 0x000C */
	/* Static information describing the journal */
	__be32	s_blocksize;		/* journal device blocksize */
	__be32	s_maxlen;		/* total blocks in journal file */
	__be32	s_first;		/* first block of log information */

/* 0x0018 */
	/* Dynamic information describing the current state of the log */
	__be32	s_sequence;		/* first commit ID expected in log */
	__be32	s_start;		/* blocknr of start of log */

/* 0x0020 */
	/* Error value, as set by jbd2_journal_abort(). */
	__be32	s_errno;

/* 0x0024 */
	/* Remaining fields are only valid in a version-2 superblock */
	__be32	s_feature_compat;	/* compatible feature set */
	__be32	s_feature_incompat;	/* incompatible feature set */
	__be32	s_feature_ro_compat;	/* readonly-compatible feature set */
/* 0x0030 */
	__u8	s_uuid[16];		/* 128-bit uuid for journal */

/* 0x0040 */
	__be32	s_nr_users;		/* Nr of filesystems sharing log */

	__be32	s_dynsuper;		/* Blocknr of dynamic superblock copy*/

/* 0x0048 */
	__be32	s_max_transaction;	/* Limit of journal blocks per trans.*/
	__be32	s_max_trans_data;	/* Limit of data blocks per trans. */

/* 0x0050 */
	__u8	s_checksum_type;	/* checksum type */
	__u8	s_padding2[3];
/* 0x0054 */
	__be32	s_num_fc_blks;		/* Number of fast commit blocks */
	__be32	s_head;			/* blocknr of head of log, only uptodate
					 * while the filesystem is clean */
/* 0x005C */
	__u32	s_padding[40];
	__be32	s_checksum;		/* crc32c(superblock) */

/* 0x0100 */
	__u8	s_users[16*48];		/* ids of all fs'es sharing the log */
/* 0x0400 */
} journal_superblock_t;

#define JBD2_FEATURE_COMPAT_CHECKSUM		0x00000001

#define JBD2_FEATURE_INCOMPAT_REVOKE		0x00000001
#define JBD2_FEATURE_INCOMPAT_64BIT		0x00000002
#define JBD2_FEATURE_INCOMPAT_ASYNC_COMMIT	0x00000004
#define JBD2_FEATURE_INCOMPAT_CSUM_V2		0x00000008
#define JBD2_FEATURE_INCOMPAT_CSUM_V3		0x00000010
#define JBD2_FEATURE_INCOMPAT_FAST_COMMIT	0x00000020

/* See "journal feature predicate functions" below */

/* Features known to this kernel version: */
#define JBD2_KNOWN_COMPAT_FEATURES	JBD2_FEATURE_COMPAT_CHECKSUM
#define JBD2_KNOWN_ROCOMPAT_FEATURES	0
#define JBD2_KNOWN_INCOMPAT_FEATURES	(JBD2_FEATURE_INCOMPAT_REVOKE | \
					JBD2_FEATURE_INCOMPAT_64BIT | \
					JBD2_FEATURE_INCOMPAT_ASYNC_COMMIT | \
					JBD2_FEATURE_INCOMPAT_CSUM_V2 | \
					JBD2_FEATURE_INCOMPAT_CSUM_V3 | \
					JBD2_FEATURE_INCOMPAT_FAST_COMMIT)

#endif	/* LINUX_JBD2_H */
