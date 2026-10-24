#include <iostream>
#include "blocks_simeon.h"
#include <cassert>

int main()
{
    {
        // Create storage for ten 4096-byte items.
        blocks test(10, 4096);
        char* data = test.getPool();

        // Check the first and last bytes of the pool.
        data[0] = 'A';
        data[40959] = 'Z';

        std::cout << data[0] << " " << data[40959] << "\n";

        // Accept the last item and reject an index outside the pool.
        // Clearing item 9 must leave item 8 marked used.
        assert(test.BMsetBit(8) == 1);
        assert(test.BMsetBit(9) == 1);
        assert(test.BMclearBit(9) == 1);
        assert(test.BMclearBit(8) == 1);

        // Reject an index outside the ten-item pool.
        assert(test.BMsetBit(10) == 0);
        assert(test.BMclearBit(10) == 0);

        std::cout << "Bitmap index checks passed.\n";
        // Searching should not mark an item used.
assert(test.findFree() == 0);
assert(test.findFree() == 0);

// Mark all ten real items used.
for (unsigned int i = 0; i < 10; i++)
{
    assert(test.BMsetBit(i) == 1);
}
assert(test.findFree() == -1);

// A freed item should become available again.
assert(test.BMclearBit(9) == 1);
assert(test.findFree() == 9);

std::cout << "Free-item search checks passed.\n";
// Reserve the last free item, then check that the pool is full.
assert(test.allocate() == 9);
assert(test.allocate() == -1);

std::cout << "Allocation checks passed.\n";
    } // Leaving this scope calls test's destructor.

    std::cout << "Object destroyed.\n";
    return 0;
}