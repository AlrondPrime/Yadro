#include "MergeSort.hpp"
#include "gtest/gtest.h"

// int32_t : -2147483648 ~ 2147483647
int main(int argc, char *argv[]) {
    try {
        ::testing::InitGoogleTest(&argc, argv);
        std::ios_base::sync_with_stdio(false);

        // Parse command-line arguments
        std::string input_file, output_file;
        bpo::options_description description("Positional arguments");
        description.add_options()
                ("input-file", bpo::value(&input_file))
                ("output-file", bpo::value(&output_file));
        bpo::positional_options_description positional_options;
        positional_options
                .add("input-file", 1)
                .add("output-file", 1);
        bpo::variables_map variables_map;
        bpo::store(bpo::command_line_parser(argc, argv)
                           .options(description)
                           .positional(positional_options).run(), variables_map);
        variables_map.notify();

        if (!std::filesystem::exists(input_file)) {
            std::cerr << "File \'" << input_file << "\' does not exist." << std::endl;
            return 0;
        }

        std::ofstream file_creator{output_file, std::fstream::trunc};
        file_creator.close();

        MergeSort::sort(input_file, output_file);
    }
    catch (std::exception &e) {
        std::cerr << e.what() << std::endl;
        return -1;
    }

    return RUN_ALL_TESTS();
}
