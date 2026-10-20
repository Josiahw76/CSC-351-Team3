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

    Block larger(17);
    assert(larger.getBitmapSize() == 3);
    std::cout << "Constructor and bitmap-size tests passed.\n";
}