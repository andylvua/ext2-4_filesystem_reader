// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
//
// Created by andrew on 11/30/23.
//

#include "filesystem/structs/dirent.h"
#include "filesystem/constants.h"


Dirent::Dirent(const FSInfo *fs_info, ext2_dir_entry *entry) : fs_info(fs_info), entry(entry),
                                                               inode(fs_info, entry->inode) {
}

Dirent::Dirent(const FSInfo *fs_info, inode_block_t inode_num, std::string name) : fs_info(fs_info),
                                                                                   inode(fs_info, inode_num),
                                                                                   name(std::move(name)) {
    entry = nullptr;
}


std::string Dirent::get_name() const {
    if (entry == nullptr) {
        return name;
    }
    return {entry->name, static_cast<size_t>(entry->name_len & 0xFF)};
}

int Dirent::get_type() const {
    if (entry == nullptr) {
        return EXT2_FT_DIR;
    }
    return entry->name_len >> 8;
}

bool Dirent::is_directory() const {
    return this->get_type() == EXT2_FT_DIR;
}

bool Dirent::is_valid() const {
    return inode.is_valid();
}

block_size_t Dirent::next() {
    auto offset = entry->rec_len;
    entry = reinterpret_cast<ext2_dir_entry *>(reinterpret_cast<char *>(entry) + entry->rec_len);
    inode = Inode(fs_info, entry->inode);
    return offset;
}

Inode Dirent::get_inode() const {
    return inode;
}

void Dirent::print() const {
    fmt::print("{:<25} ", format_time(inode->i_ctime));
    fmt::print("{:<25} ", format_time(inode->i_mtime));
    fmt::print("{:<5} ", format_flags(file_type_map, this->get_type(), false));

    if (this->is_directory()) {
        fmt::print("{:<10} ", "-");
    } else {
        fmt::print("{:<10} ", inode.get_size());
    }

    fmt::print("{:1}{:1}{:1}{:1}{:1}{:1}{:1}{:1}{:1}{:1}  ",
               (inode->i_mode & S_IFDIR ? "d" : "-"),
               (inode->i_mode & S_IRUSR ? "r" : "-"),
               (inode->i_mode & S_IWUSR ? "w" : "-"),
               (inode->i_mode & S_IXUSR ? "x" : "-"),
               (inode->i_mode & S_IRGRP ? "r" : "-"),
               (inode->i_mode & S_IWGRP ? "w" : "-"),
               (inode->i_mode & S_IXGRP ? "x" : "-"),
               (inode->i_mode & S_IROTH ? "r" : "-"),
               (inode->i_mode & S_IWOTH ? "w" : "-"),
               (inode->i_mode & S_IXOTH ? "x" : "-"));

    fmt::print("{:<5} ", inode->i_uid);
    fmt::print("{:<6} ", inode->i_links_count);
    auto const first_block = inode.get_block(0);
    fmt::print("{:<10} ", first_block);
    fmt::print("{:<10} ", first_block * fs_info->block_size / SECTOR_SIZE);
    fmt::print("{:<12} ", format_flags(inode_flags_map, inode->i_flags));
    fmt::print("{}\n", this->get_name());
}

void Dirent::print_header() {
    fmt::print("{:<25} {:<25} {:<5} {:<10} {:<11} {:<5} {:<6} {:<10} {:<10} {:<12} {}\n",
               "Created", "Modified", "Type", "Size", "Mode", "UID", "Links", "Block", "Sector", "Flags", "Name");
}

bool operator==(const Dirent &lhs, const Dirent &rhs) {
    return lhs.entry == rhs.entry && lhs.inode == rhs.inode;
}
