// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
//
// Created by andrew on 11/26/23.
//

#ifndef FS_READER_GPT_H
#define FS_READER_GPT_H

#include <cstdint>
#include <iostream>
#include <codecvt>


struct GPT {
    struct Partition {
        uint8_t type_guid[16];
        uint8_t unique_guid[16];
        uint64_t first_lba;
        uint64_t last_lba;
        uint64_t attributes;
        char partition_name[72];

        [[nodiscard]] bool is_used() const;
        void print() const;
    } __attribute__((packed));

    [[maybe_unused]] uint64_t signature;
    [[maybe_unused]] uint32_t revision;
    [[maybe_unused]] uint32_t header_size;
    [[maybe_unused]] uint32_t header_crc32;
    [[maybe_unused]] uint32_t reserved;
    [[maybe_unused]] uint64_t current_lba;
    [[maybe_unused]] uint64_t backup_lba;
    [[maybe_unused]] uint64_t first_usable_lba;
    [[maybe_unused]] uint64_t last_usable_lba;
    [[maybe_unused]] uint8_t disk_guid[16];
    uint64_t partition_table_lba;
    uint32_t partition_count;
    [[maybe_unused]] uint32_t partition_entry_size;
    [[maybe_unused]] uint32_t partition_table_crc32;
    uint8_t reserved2[420];
} __attribute__((packed));


[[nodiscard]] inline bool GPT::Partition::is_used() const {
    return type_guid[0] != 0;
}

inline void GPT::Partition::print() const {
    std::cout << "Type GUID: " << format_uuid(type_guid) << "\n";
    std::cout << "Unique GUID: " << format_uuid(unique_guid) << "\n";
    std::cout << "First LBA: " << first_lba << "\n";
    std::cout << "Last LBA: " << last_lba << "\n";
    std::cout << "Attributes: " << attributes << "\n";
    std::wstring_convert<std::codecvt_utf8_utf16<char16_t>, char16_t> convert;
    std::u16string u16_name(reinterpret_cast<const char16_t *>(partition_name));
    std::cout << "Partition name: " << convert.to_bytes(u16_name) << std::endl;
}

#endif //FS_READER_GPT_H
