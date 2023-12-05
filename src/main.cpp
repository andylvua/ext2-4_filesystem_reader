// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include "filesystem/filesystem_reader.h"
#include "cl_options/options_parser.h"
#include "errors/exception.h"


namespace {
    void print_info(const OptionsParser &options, const FS_Reader &filesystem) {
        if (options.should_print_superblock()) {
            filesystem.print_superblock();
        }
        if (options.should_print_root()) {
            std::cout << std::endl;
            filesystem.print_root(options.is_recursive());
        }

        auto print_journal_super = options.should_print_journal_super();
        auto print_transactions = options.should_print_transactions();
        if (print_journal_super || print_transactions) {
            filesystem.print_journal(print_journal_super, print_transactions);
        }
    }
}


int main(int argc, char *argv[]) {
    const OptionsParser options(argc, argv);

    std::string image_path = options.get_image_path();
    int partition_no = options.get_partition();

    try {
        DiskImage image(image_path);
        if (options.should_print_partitions()) {
            image.print_partitions();
            return EXIT_SUCCESS;
        }
        if (partition_no >= 0) {
            FS_Reader filesystem(image.get_partition(partition_no));
            print_info(options, filesystem);
            return EXIT_SUCCESS;
        }

        FS_Reader filesystem(image.to_filesystem());
        print_info(options, filesystem);
    } catch (FSReaderException &ex) {
        std::cerr << ex.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
