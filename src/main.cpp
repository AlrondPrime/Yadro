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

        MergeSort::sort("../tapeIn.txt", "../tapeOut.txt");
    }
    catch (std::exception &e) {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}
