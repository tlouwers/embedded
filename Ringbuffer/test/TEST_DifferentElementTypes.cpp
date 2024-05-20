
#include "../../Catch/catch.hpp"

#include "Ringbuffer.hpp"


TEST_CASE( "Ringbuffer different element types - TwoTypes", "[Ringbuffer]" )
{
    struct TwoTypes
    {
        int16_t myInt16;
        float   myFloat;
    };

    // For each section, Ringbuffer ringBuff is anew:
    Ringbuffer<TwoTypes> ringBuff;

    TwoTypes src[4] = { };
    TwoTypes dest[4] = { };
    TwoTypes* pSrc = &src[0];
    TwoTypes* pDest = &dest[0];

    REQUIRE(ringBuff.Resize(3) == true);
    REQUIRE(ringBuff.Size() == 0);


    SECTION( "push and pop data - blocksize 1" )
    {
        for (uint16_t i = 1; i <= 100; i++)
        {
            // Add block with size 1
            pSrc[0].myInt16 = i;
            pSrc[0].myFloat = i + 0.5f;
            REQUIRE(ringBuff.TryPush(pSrc, 1) == true);

            REQUIRE(ringBuff.TryPop(pDest, 1) == true);
            REQUIRE(pDest[0].myInt16 == i);
            REQUIRE(pDest[0].myFloat == Approx(i + 0.5f));
        }

        REQUIRE(ringBuff.Size() == 0);
    }

    SECTION( "push and pop data - blocksize 2" )
    {
        for (uint16_t i = 1; i <= 100; i++)
        {
            // Add block with size 2
            pSrc[0].myInt16 = i;
            pSrc[0].myFloat = i + 0.5f;
            pSrc[1].myInt16 = i + 1;
            pSrc[1].myFloat = i + 1.5f;
            REQUIRE(ringBuff.TryPush(pSrc, 2) == true);

            REQUIRE(ringBuff.TryPop(pDest, 2) == true);
            REQUIRE(pDest[0].myInt16 == i);
            REQUIRE(pDest[0].myFloat == Approx(i + 0.5f));
            REQUIRE(pDest[1].myInt16 == (i + 1));
            REQUIRE(pDest[1].myFloat == Approx(i + 1.5f));
        }

        REQUIRE(ringBuff.Size() == 0);
    }
}


TEST_CASE( "Ringbuffer different element types - TypeAndArray", "[Ringbuffer]" )
{
    struct TypeAndArray
    {
        uint32_t myInt32;
        uint8_t  myArray[3];
    };

    // For each section, Ringbuffer ringBuff is anew:
    Ringbuffer<TypeAndArray> ringBuff;

    TypeAndArray src[4] = { };
    TypeAndArray dest[4] = { };
    TypeAndArray* pSrc = &src[0];
    TypeAndArray* pDest = &dest[0];

    REQUIRE(ringBuff.Resize(3) == true);
    REQUIRE(ringBuff.Size() == 0);


    SECTION( "push and pop data - blocksize 1" )
    {
        for (uint32_t i = 1; i <= 100; i++)
        {
            // Add block with size 1
            pSrc[0].myInt32 = i;
            pSrc[0].myArray[0] = i + 1;
            pSrc[0].myArray[1] = i + 2;
            pSrc[0].myArray[2] = i + 3;
            REQUIRE(ringBuff.TryPush(pSrc, 1) == true);

            REQUIRE(ringBuff.TryPop(pDest, 1) == true);
            REQUIRE(pDest[0].myInt32 == i);
            REQUIRE(pDest[0].myArray[0] == (i + 1));
            REQUIRE(pDest[0].myArray[1] == (i + 2));
            REQUIRE(pDest[0].myArray[2] == (i + 3));
        }

        REQUIRE(ringBuff.Size() == 0);
    }

    SECTION( "push and pop data - blocksize 2" )
    {
        for (uint32_t i = 1; i <= 100; i++)
        {
            // Add block with size 2
            pSrc[0].myInt32 = i;
            pSrc[0].myArray[0] = i + 1;
            pSrc[0].myArray[1] = i + 2;
            pSrc[0].myArray[2] = i + 3;
            pSrc[1].myInt32 = i + 1;
            pSrc[1].myArray[0] = i + 2;
            pSrc[1].myArray[1] = i + 3;
            pSrc[1].myArray[2] = i + 4;
            REQUIRE(ringBuff.TryPush(pSrc, 2) == true);

            REQUIRE(ringBuff.TryPop(pDest, 2) == true);
            REQUIRE(pDest[0].myInt32 == i);
            REQUIRE(pDest[0].myArray[0] == (i + 1));
            REQUIRE(pDest[0].myArray[1] == (i + 2));
            REQUIRE(pDest[0].myArray[2] == (i + 3));
            REQUIRE(pDest[1].myInt32 == (i + 1));
            REQUIRE(pDest[1].myArray[0] == (i + 2));
            REQUIRE(pDest[1].myArray[1] == (i + 3));
            REQUIRE(pDest[1].myArray[2] == (i + 4));
        }

        REQUIRE(ringBuff.Size() == 0);
    }
}
