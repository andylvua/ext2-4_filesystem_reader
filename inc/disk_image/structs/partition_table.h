// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
//
// Created by andrew on 11/26/23.
//

#ifndef FS_READER_PARTITION_TABLE_H
#define FS_READER_PARTITION_TABLE_H

#include "mbr.h"
#include "gpt.h"
#include "filesystem/filesystem_dump.h"

#include <variant>
#include <memory>
#include <vector>


enum class PartitioningType {
    MBR,
    GPT,
    UNKNOWN,
    NONE
};

class PartitionTable {
    using partitions_t = std::vector<std::variant<MBR::Partition, GPT::Partition>>;

public:
    PartitionTable() = default;

    explicit PartitionTable(std::shared_ptr<ImageDump> dump);

    [[nodiscard]] bool has_partitions() const;

    void print_partitions() const;

    [[nodiscard]] FilesystemDump get_partition(size_t partition_num) const;

private:
    void read_partitions();

    MBR *mbr = nullptr;
    PartitioningType partitioning_type = PartitioningType::UNKNOWN;
    partitions_t partitions;
    std::shared_ptr<ImageDump> dump;
};

#endif //FS_READER_PARTITION_TABLE_H
