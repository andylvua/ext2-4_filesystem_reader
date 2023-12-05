// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
//
// Created by andrew on 11/26/23.
//

#ifndef FS_READER_MBR_H
#define FS_READER_MBR_H

#include "mappings/flag_mappings.h"
#include "utils/utils.h"

#include <cstdint>
#include <iostream>

static constexpr auto MBR_BOOT_SIGNATURE = 0xAA55;
static constexpr auto GPT_PARTITION_TYPE = 0xEE;
static constexpr auto MBR_EXTENDED_PARTITION_TYPE = 0x05;
static constexpr auto MBR_EXTENDED_PARTITION_TYPE_LBA = 0x0F;


struct MBR {
    struct Partition {
        uint8_t bootable;
        uint8_t start_chs[3];
        uint8_t partition_type;
        uint8_t end_chs[3];
        uint32_t start_lba;
        uint32_t size;

        [[nodiscard]] bool is_used() const;
        [[nodiscard]] bool is_extended() const;
        void print() const;
    } __attribute__((packed));

    [[maybe_unused]] char bootstrap_code[446];
    Partition partition_table[4];
    uint16_t boot_signature;

    [[nodiscard]] bool is_valid() const;
    [[nodiscard]] bool is_protective() const;
} __attribute__((packed));


[[nodiscard]] inline bool MBR::Partition::is_used() const {
    return partition_type != 0;
}

[[nodiscard]] inline bool MBR::Partition::is_extended() const {
    return partition_type == MBR_EXTENDED_PARTITION_TYPE;
}

inline void MBR::Partition::print() const {
    auto format_chs = [](const uint8_t chs[3]) {
        std::stringstream chs_stream;
        chs_stream << std::hex << std::uppercase << std::setfill('0');
        for (int j = 0; j < 3; ++j) {
            chs_stream << std::setw(2) << static_cast<int>(chs[j]);
            if (j != 2) {
                chs_stream << ":";
            }
        }
        return chs_stream.str();
    };

    std::cout << "Bootable: " << static_cast<int>(bootable) << "\n";
    std::cout << "Start CHS: " << format_chs(start_chs) << "\n";
    std::cout << "Partition type: " << format_flags(partition_types_map, partition_type, false) << "\n";
    std::cout << "End CHS: " << format_chs(end_chs) << "\n";
    std::cout << "Start LBA: " << start_lba << "\n";
    std::cout << "Size: " << size << "\n";
}


[[nodiscard]] inline bool MBR::is_valid() const {
    return boot_signature == MBR_BOOT_SIGNATURE;
}

[[nodiscard]] inline bool MBR::is_protective() const {
    return partition_table[0].partition_type == GPT_PARTITION_TYPE;
}

#endif //FS_READER_MBR_H
