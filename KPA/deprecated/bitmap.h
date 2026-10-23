#pragma once

class bitmap {
    private:
        unsigned int itemCount = 0;
        unsigned int bitmapSize = 0;
        unsigned char *bits;
        unsigned int bitmapBytes(unsigned int itemCount)
        {
            unsigned int bytes = itemCount / 8;
            unsigned int leftover = itemCount % 8;

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
        int findFree()
        {
            for (unsigned int byteIndex = 0;
                byteIndex < bitmapSize;
                byteIndex++)
            {
                if (bits[byteIndex] !=255)
                {
                    unsigned int freePosition = 0;

                    while (freePosition < 8 &&
                        ((bits[byteIndex] >> freePosition) & 1) == 1)
                    {
                        freePosition++;
                    }

                    unsigned int index = byteIndex * 8 + freePosition;

                    if (index < itemCount)
                    {
                        return index;
                    }
                }
            }


            return -1;

        }


        bitmap(unsigned int count)
        {
            itemCount = count;
            bitmapSize = bitmapBytes(count);

            bits = new unsigned char[bitmapSize];
        
            for (unsigned int i = 0; i < bitmapSize; i++)
            {
                bits[i] = 0;
            }
        }

        unsigned int getItemCount() const
        {
            return itemCount;
        }

        unsigned int getBitmapSize() const

        {
            
            return bitmapSize;
        }

        ~bitmap()
        {
            delete[] bits;
        }


};

