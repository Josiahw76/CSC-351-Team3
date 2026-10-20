#pragma once

class Block {
    private:
        unsigned int blockCount = 0;
        unsigned int bitmapSize = 0;
        unsigned int bitmapBytes(unsigned int blockCount)
        {
            unsigned int bytes = blockCount / 8;
            unsigned int leftover = blockCount % 8;

            if (leftover > 0)
            {
                bytes = bytes + 1;
            }

            return bytes;
        }
    public:
        Block(unsigned int count)
        {
            blockCount = count;
            bitmapSize = bitmapBytes(count);
        }

        unsigned int getBlockCount() const
        {
            return blockCount;
        }

        unsigned int getBitmapSize() const
        {
            return bitmapSize;
        }


};

