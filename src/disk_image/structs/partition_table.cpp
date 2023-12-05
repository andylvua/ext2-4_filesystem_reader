//
// Created by andrew on 11/30/23.
//

#include "disk_image/structs/partition_table.h"
#include "errors/exception.h"
#include "filesystem/constants.h"

#include <iostream>


PartitionTable::PartitionTable(std::shared_ptr<ImageDump> dump) : dump(std::move(dump)) {
    mbr = this->dump->data<MBR>();
    if (mbr->is_valid()) {
        partitioning_type = PartitioningType::MBR;
        if (mbr->is_protective()) {
            partitioning_type = PartitioningType::GPT;
        }
    } else {
        partitioning_type = PartitioningType::NONE;
    }

    read_partitions();
}

bool PartitionTable::has_partitions() const {
    return partitioning_type != PartitioningType::NONE;
}

void PartitionTable::print_partitions() const {
    if (partitioning_type == PartitioningType::NONE) {
        std::cerr << "fs_reader: Error: No partition table found" << std::endl;
        return;
    }

    std::cout << "Partitioning type: " << (partitioning_type == PartitioningType::MBR ? "MBR" : "GPT") << "\n\n";
    int i = 0;
    for (auto const &partition: partitions) {
        std::cout << "Partition: " << i++ << "\n";
        std::visit([](auto &&arg) { arg.print(); }, partition);
        std::cout << "\n";
    }
}

FilesystemDump PartitionTable::get_partition(const size_t partition_num) const {
    if (!has_partitions()) {
        throw FSReaderException("No partition table found");
    }
    if (partition_num >= partitions.size()) {
        throw FSReaderException("No such partition: " + std::to_string(partition_num));
    }

    if (partitioning_type == PartitioningType::MBR) {
        auto const &partition = std::get<MBR::Partition>(partitions[partition_num]);
        if (partition.is_extended()) {
            throw FSReaderException("Cannot read extended partition directly");
        }
        return FilesystemDump(dump, partition.start_lba * SECTOR_SIZE, partition.size * SECTOR_SIZE);
    }
    if (partitioning_type == PartitioningType::GPT) {
        auto const &partition = std::get<GPT::Partition>(partitions[partition_num]);
        auto const size_lba = partition.last_lba - partition.first_lba + 1;
        return FilesystemDump(dump, partition.first_lba * SECTOR_SIZE, size_lba * SECTOR_SIZE);
    }

    throw FSReaderException("Unknown partitioning type");
}

void PartitionTable::read_partitions() {
    if (partitioning_type == PartitioningType::MBR) {
        for (auto &partition: mbr->partition_table) {
            if (!partition.is_used()) {
                continue;
            }
            partitions.emplace_back(partition);
            if (partition.partition_type == MBR_EXTENDED_PARTITION_TYPE ||
                partition.partition_type == MBR_EXTENDED_PARTITION_TYPE_LBA) {
                auto const *ebr = dump->data<MBR>(partition.start_lba * SECTOR_SIZE);
                MBR::Partition next_ebr{
                        .bootable = 0,
                        .start_chs = {0, 0, 0},
                        .partition_type = partition.partition_type,
                        .end_chs = {0, 0, 0},
                        .start_lba = 0,
                        .size = 0
                };
                while (next_ebr.is_used()) {
                    auto logical_partition = ebr->partition_table[0];
                    if (logical_partition.is_used()) {
                        logical_partition.start_lba += partition.start_lba + next_ebr.start_lba;
                        partitions.emplace_back(logical_partition);
                    }
                    next_ebr = ebr->partition_table[1];
                    ebr = dump->data<MBR>((partition.start_lba + next_ebr.start_lba) * SECTOR_SIZE);
                }
            }
        }
    } else if (partitioning_type == PartitioningType::GPT) {
        auto const *gpt = dump->data<GPT>(mbr->partition_table[0].start_lba * SECTOR_SIZE);
        auto *partition_table = dump->data<GPT::Partition>(gpt->partition_table_lba * SECTOR_SIZE);
        for (uint32_t i = 0; i < gpt->partition_count; ++i) {
            if (partition_table[i].is_used()) {
                partitions.emplace_back(partition_table[i]);
            }
        }
    }
}
