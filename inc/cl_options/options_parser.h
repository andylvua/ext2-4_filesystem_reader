//
// Created by andrew on 11/26/23.
//

#ifndef OPTIONS_PARSER_H
#define OPTIONS_PARSER_H

#include <iostream>
#include <boost/program_options.hpp>

namespace po = boost::program_options;


class OptionsParser {
public:
    OptionsParser(const int argc, char *argv[]) {
        try {
            desc.add_options()
                    ("help,h", "Print this help message and exit")
                    ("image,i", po::value<std::string>(&image_path)->required(),
                     "Path to disk image (required)\n"
                     "Can be specified as a positional argument if no other options are used.")
                    ("partition,p", po::value<int>(&partition_number),
                     "Number of partition to read (optional, positive number). "
                     "Only valid if image is partitioned.\n")
                    ("superblock,s", po::bool_switch(&print_superblock),
                     "Print superblock information")
                    ("root,r", po::bool_switch(&print_root),
                     "Print root directory contents")
                    ("recursive,R", po::bool_switch(&recursive),
                     "Enable recursive printing. Requires -r/--root")
                    ("journal,j", po::bool_switch(&print_journal_super),
                     "Print journal superblock information")
                    ("transactions,t", po::bool_switch(&print_transactions),
                     "Print journal transactions")
                    ("partitions,P", po::bool_switch(&print_partitions),
                     "Print partition table. Overrides all other options. "
                     "You can use this option to retrieve partition indexes.");

            po::positional_options_description p;
            p.add("image", 1);

            store(po::command_line_parser(argc, argv)
                          .options(desc)
                          .positional(p)
                          .run(), vm);

            if (vm.contains("help")) {
                usage();
            }

            notify(vm);
            validate();
        } catch (const po::error &e) {
            std::cerr << "fs_reader: error parsing command line arguments: " << e.what() << "\n" << std::endl;
            usage(true);
        }
    }

    void usage(const bool failure = false) const {
        std::cout << "Usage: fs_reader [options]\n" << desc << std::endl;
        exit(failure ? EXIT_FAILURE : EXIT_SUCCESS);
    }

    [[nodiscard]] std::string get_image_path() const {
        return image_path;
    }

    [[nodiscard]] int get_partition() const {
        return vm.contains("partition") ? partition_number : -1;
    }

    [[nodiscard]] bool should_print_superblock() const {
        return print_superblock;
    }

    [[nodiscard]] bool should_print_journal_super() const {
        return print_journal_super;
    }

    [[nodiscard]] bool should_print_root() const {
        return print_root;
    }

    [[nodiscard]] bool should_print_transactions() const {
        return print_transactions;
    }

    [[nodiscard]] bool is_recursive() const {
        return recursive;
    }

    [[nodiscard]] bool should_print_partitions() const {
        return print_partitions;
    }

private:
    void validate() const {
        if (vm.contains("partition") && partition_number < 0) {
            throw po::validation_error(po::validation_error::invalid_option_value,
                                       "partition",
                                       std::to_string(partition_number));
        }

        if (recursive && !print_root) {
            throw po::error("Recursive printing requires -r/--root");
        }
    }

    po::variables_map vm;
    std::string image_path;
    int partition_number = -1;
    bool print_superblock = false;
    bool print_journal_super = false;
    bool print_root = false;
    bool print_transactions = false;
    bool recursive = false;
    bool print_partitions = false;
    po::options_description desc = po::options_description("Allowed options");
};

#endif //OPTIONS_PARSER_H
