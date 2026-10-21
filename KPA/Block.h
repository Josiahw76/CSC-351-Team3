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

        unsigned int findFreeBlock()
        {
            for (unsigned int byteIndex = 0;
                byteIndex < bitmapSize;
                byteIndex++)
            {
                if (dataBlockBitMap[byteIndex] !=255)
                {
                    unsigned int freePosition = 0;

                    while (freePosition < 8 &&
                        ((dataBlockBitMap[byteIndex] >> freePosition) & 1) == 1)
                    {
                        freePosition++;
                    }

                    unsigned int blockIndex = byteIndex * 8 + freePosition;

                    if (blockIndex < blockCount)
                    {
                        return blockIndex;
                    }
                }
            }


            return blockCount;

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

