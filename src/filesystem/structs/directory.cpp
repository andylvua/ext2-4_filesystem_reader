// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
//
// Created by andrew on 11/30/23.
//

#include "filesystem/structs/directory.h"


Directory::Directory(const FSInfo *fs_info, const Inode& inode, std::string path) : fs_info(fs_info),
                                                                                    inode(inode),
                                                                                    path(std::move(path)) {}

Directory::iterator Directory::begin() const {
    return {fs_info, inode.begin()};
}

Directory::iterator Directory::end() const {
    return {fs_info, inode.end()};
}

void Directory::print(bool recursive) const { // NOLINT
    std::vector<Dirent> entries_dirs;

    std::cout << path + ":\n";

    auto end = this->end();
    for (auto const &dirent: *this) {
        dirent.print();
        if (auto name = dirent.get_name(); dirent.is_directory() && name != "." && name != "..") {
            entries_dirs.push_back(dirent);
        }
    }

    if (recursive) {
        for (auto const &dirent: entries_dirs) {
            auto const new_path = path + dirent.get_name() + "/";
            std::cout << "\n";
            Directory(fs_info, dirent.get_inode(), new_path).print(true);
        }
    }
}
