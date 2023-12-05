//
// Created by andrew on 12/1/23.
//

#ifndef FS_READER_TYPES_H
#define FS_READER_TYPES_H

#include <cstdint>
#include <vector>
#include "disk_image/disk_image_dump.h"


using block_t = uint64_t;
using block_size_t = uint16_t;

using data_t = ImageDump::data_t;

#endif //FS_READER_TYPES_H
