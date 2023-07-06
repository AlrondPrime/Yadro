#include "BinaryTape.hpp"
#include "TextTape.hpp"
#include "MergeSort.hpp"

// int32_t : -2147483648 ~ 2147483647
int main(int argc, char *argv[]) {
    try {
        std::ios_base::sync_with_stdio(false);

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

        // Generate some test data
        {
            TextTape textTape{input_file};
            textTape.clear();

            textTape.write(1);
            textTape.forward();
            textTape.write(7);
            textTape.forward();
            textTape.write(2);
            textTape.forward();
            textTape.write(6);
            textTape.forward();
            textTape.write(3);
            textTape.forward();
            textTape.write(5);
            textTape.forward();
            textTape.write(4);
        }

        MergeSort::sort(input_file, output_file);
    }
    catch (std::exception &e) {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}
