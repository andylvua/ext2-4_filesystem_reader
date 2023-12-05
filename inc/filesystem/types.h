// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
//
// Created by andrew on 12/1/23.
//

#ifndef FS_READER_TYPES_H
#define FS_READER_TYPES_H

#include <cstdint>
#include <vector>


using block_t = uint64_t;
using block_size_t = uint16_t;

using data_t = std::vector<char>;

#endif //FS_READER_TYPES_H
