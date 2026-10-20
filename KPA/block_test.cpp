#include "Block.h"
#include <cassert>
#include <iostream>

int main()
{
    Block first(10);
    assert(first.getBlockCount() == 10);
    assert(first.getBitmapSize() == 2);

    Block second (16);
    assert(second.getBlockCount() == 16);
    assert(second.getBitmapSize() == 2);

    unsigned char byte = 1;
    unsigned int bitPosition = 2;
    unsigned char mask = 1 << bitPosition;
    byte = byte | mask;
    assert(byte == 5);
    std::cout << "Bit-setting test passed.\n";

    byte = byte & ~mask;
    assert(byte == 1);
    std::cout << "Bit-clearing test passed.\n";

    unsigned char searchByte = 5;
    unsigned int freePosition = 0;

    while (freePosition < 8 &&
            ((searchByte >> freePosition) & 1) == 1)
    {
        freePosition++;
    }

    assert(freePosition == 1);
    std::cout << "Free-bit search test passed.\n";

    Block larger(17);
    assert(larger.getBitmapSize() == 3);
    std::cout << "Constructor and bitmap-size tests passed.\n";
}