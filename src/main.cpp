#include "pch.h"
#include "TextTape.hpp"
#include "BinaryTape.hpp"

// int32_t : -2147483648 ~ 2147483647
int main() {
    BinaryTape tape{"../tape.bin"};
//    TextTape tape{"../tape.txt"};

    tape.write(-9999);
    tape.forward();
    tape.write(2147483647);
    std::cout << "Val: " << tape.read() << std::endl;
    tape.backward();
    std::cout << "Val: " << tape.read() << std::endl;

    return 0;
}
