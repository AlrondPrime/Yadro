#include "BinaryTape.hpp"
#include "TextTape.hpp"
#include "MergeSort.hpp"

// int32_t : -2147483648 ~ 2147483647
int main() {
    try {
        // Generate some test data
        {
            TextTape textTape{"../tapeIn.txt"};
            textTape.clear();

            textTape.write(256);
            textTape.forward();
            textTape.write(999);
            textTape.forward();
            textTape.write(128);
            textTape.forward();
            textTape.write(128);
            textTape.forward();
            textTape.write(323);
            textTape.forward();
            textTape.write(-299);
            textTape.forward();
            textTape.write(-1000);
        }

        MergeSort::sort("../tapeIn.txt", "../tapeOut.txt");
    }
    catch (std::exception &e) {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}
