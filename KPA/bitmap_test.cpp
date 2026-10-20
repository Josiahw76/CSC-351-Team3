#include "bitmap.h"
#include <cassert>
#include <iostream>

int main()
{
    bitmap blocks(10);
    bitmap inodes(17);

    assert(blocks.getBitmapSize() == 2);
    assert(inodes.getBitmapSize() == 3);

    assert(blocks.findFree() == 0);
    assert(inodes.findFree() == 0);
    assert(blocks.findFree() == 0);

    std::cout << "Bitmap construction and search tests passed.\n";
}