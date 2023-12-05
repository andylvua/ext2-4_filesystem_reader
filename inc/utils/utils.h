// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
//
// Created by andrew on 11/24/23.
//

#ifndef FS_READER_UTILS_H
#define FS_READER_UTILS_H

#include "mappings/flag_mappings.h"

#include <map>
#include <string>
#include <sstream>
#include <ctime>
#include <climits>
#include <iomanip>


#ifdef __GNUC__

#include <byteswap.h>

#endif

template<typename T>
T swap_endian(T u) {
    static_assert(CHAR_BIT == 8, "CHAR_BIT != 8");

    if constexpr (sizeof(T) == 1) {
        return u;
    } else if constexpr (sizeof(T) == 2) {
        return __bswap_16(u);
    } else if constexpr (sizeof(T) == 4) {
        return __bswap_32(u);
    } else if constexpr (sizeof(T) == 8) {
        return __bswap_64(u);
    } else {
        static_assert(sizeof(T) == 1 || sizeof(T) == 2 || sizeof(T) == 4 || sizeof(T) == 8,
                      "Unsupported type");
    }
}

inline std::string format_flags(const flags_map_decltype& map, const uint32_t flags, const bool binary = true) {
    std::stringstream flags_stream;

    if (binary) {
        if (flags == 0) {
            flags_stream << "none";
            return flags_stream.str();
        }
        for (auto const& [flag, name] : map) {
            if (flags & flag) {
                flags_stream << name << " ";
            }
        }
    } else {
        flags_stream << map.at(flags);
    }

    return flags_stream.str();
}

inline std::string format_time(uint64_t timestamp) {
    std::string buffer;
    buffer.resize(24);
    auto const time = static_cast<std::time_t>(timestamp);
    std::strftime(buffer.data(), buffer.size() + 1, "%c", std::localtime(&time));
    return buffer;
}

inline std::string format_uuid(const uint8_t uuid[16]) {
    std::stringstream uuid_stream;
    uuid_stream << std::hex << std::setfill('0');
    for (int i = 0; i < 16; i++) {
        uuid_stream << std::setw(2) << static_cast<int>(uuid[i]);
        if (i == 3 || i == 5 || i == 7 || i == 9) {
            uuid_stream << "-";
        }
    }
    return uuid_stream.str();
}

#endif //FS_READER_UTILS_H
