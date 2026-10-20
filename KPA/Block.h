#pragma once

class Block {
    private:
        unsigned int blockCount = 0;
        unsigned int bitmapSize = 0;
        unsigned char *dataBlockBitMap;
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
        unsigned char setBit(unsigned char byte, unsigned int bitPosition)
        {
            unsigned char mask = 1 << bitPosition;
            return byte | mask;
        }
        unsigned char clearBit(unsigned char byte, unsigned int bitPosition)
        {
            unsigned char mask = 1 << bitPosition;
            return byte & ~mask;
        }
    public:
        Block(unsigned int count)
        {
            blockCount = count;
            bitmapSize = bitmapBytes(count);

            dataBlockBitMap = new unsigned char[bitmapSize];
        
            for (unsigned int i = 0; i < bitmapSize; i++)
            {
                dataBlockBitMap[i] = 0;
            }
        }

        unsigned int getBlockCount() const
        {
            return blockCount;
        }

        unsigned int getBitmapSize() const

        {
            
            return bitmapSize;
        }

        ~Block()
        {
            delete[] dataBlockBitMap;
        }


};

