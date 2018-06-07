
#include "../../Catch/catch.hpp"

#include "Ringbuffer.hpp"


static Ringbuffer<int> ringBuff_ext;


// Helper method to add an element to buffer
static bool AddOne(int val)
{
    bool result = true;

    result &= ringBuff_ext.TryPush(&val);
    REQUIRE(result == true);

    return result;
}

// Helper method to remove an element from buffer
static bool RemoveOne(int& val)
{
    bool result = true;

    int tmp[1] = { -1 };
    int *pTmp = &tmp[0];

    result &= ringBuff_ext.TryPop(pTmp);
    REQUIRE(result == true);

    val = (result) ? tmp[0] : -1;

    return result;
}


TEST_CASE( "Ringbuffer TryPush() and TryPop() operations", "[Ringbuffer]" )
{
    // For each section, Ringbuffer ringBuff is anew:
    Ringbuffer<int> ringBuff;

    int src[10] = { };
    int dest[10] = { };
    int* pSrc = &src[0];
    int* pDest = &dest[0];

    REQUIRE(ringBuff.Resize(10) == true);
    REQUIRE(ringBuff.Size() == 0);


    SECTION( "push and pop data - blocksize 1" )
    {
        for (int i = 1; i <= 100; i++)
        {
            // Add block with size 1
            pSrc[0] = i;
            REQUIRE(ringBuff.TryPush(pSrc, 1) == true);

            REQUIRE(ringBuff.TryPop(pDest, 1) == true);
            REQUIRE(pDest[0] == i);
        }

        REQUIRE(ringBuff.Size() == 0);
    }

    SECTION( "push and pop data - blocksize 2" )
    {
        for (int i = 1; i <= 100; i++)
        {
            // Add block with size 2
            pSrc[0] = i;
            pSrc[1] = i + 1;
            REQUIRE(ringBuff.TryPush(pSrc, 2) == true);

            REQUIRE(ringBuff.TryPop(pDest, 2) == true);
            REQUIRE(pDest[0] == i);
            REQUIRE(pDest[1] == (i + 1));
        }

        REQUIRE(ringBuff.Size() == 0);
    }

    SECTION( "push and pop data - blocksize 3" )
    {
        for (int i = 1; i <= 100; i++)
        {
            // Add block with size 3
            pSrc[0] = i;
            pSrc[1] = i + 1;
            pSrc[2] = i + 2;
            REQUIRE(ringBuff.TryPush(pSrc, 3) == true);

            REQUIRE(ringBuff.TryPop(pDest, 3) == true);
            REQUIRE(pDest[0] == i);
            REQUIRE(pDest[1] == (i + 1));
            REQUIRE(pDest[2] == (i + 2));
        }

        REQUIRE(ringBuff.Size() == 0);
    }

    SECTION( "push and pop data - blocksize 4" )
    {
        for (int i = 1; i <= 100; i++)
        {
            // Add block with size 3
            pSrc[0] = i;
            pSrc[1] = i + 1;
            pSrc[2] = i + 2;
            pSrc[3] = i + 3;
            REQUIRE(ringBuff.TryPush(pSrc, 4) == true);

            REQUIRE(ringBuff.TryPop(pDest, 4) == true);
            REQUIRE(pDest[0] == i);
            REQUIRE(pDest[1] == (i + 1));
            REQUIRE(pDest[2] == (i + 2));
            REQUIRE(pDest[3] == (i + 3));
        }

        REQUIRE(ringBuff.Size() == 0);
    }

    SECTION( "push and pop data - deep inspection" )
    {
        REQUIRE(ringBuff_ext.Resize(3) == true);
        REQUIRE(ringBuff_ext.Size() == 0);

        // -----

        int index = 1;
        int val   = -1;

        REQUIRE(ringBuff_ext.CheckState(0, 0) == true);

        REQUIRE(AddOne(index++) == true);
        REQUIRE(ringBuff_ext.CheckState(1, 0) == true);

        REQUIRE(RemoveOne(val) == true);
        REQUIRE(ringBuff_ext.CheckState(1, 1) == true);
        REQUIRE(val == 1);

        REQUIRE(AddOne(index++) == true);
        REQUIRE(ringBuff_ext.CheckState(2, 1) == true);

        REQUIRE(RemoveOne(val) == true);
        REQUIRE(ringBuff_ext.CheckState(2, 2) == true);
        REQUIRE(val == 2);

        REQUIRE(AddOne(index++) == true);
        REQUIRE(ringBuff_ext.CheckState(3, 2) == true);

        REQUIRE(RemoveOne(val) == true);
        REQUIRE(ringBuff_ext.CheckState(3, 3) == true);
        REQUIRE(val == 3);

        REQUIRE(AddOne(index++) == true);
        REQUIRE(ringBuff_ext.CheckState(0, 3) == true);

        REQUIRE(RemoveOne(val) == true);
        REQUIRE(ringBuff_ext.CheckState(0, 0) == true);
        REQUIRE(val == 4);

        REQUIRE(AddOne(index++) == true);
        REQUIRE(ringBuff_ext.CheckState(1, 0) == true);

        REQUIRE(RemoveOne(val) == true);
        REQUIRE(ringBuff_ext.CheckState(1, 1) == true);
        REQUIRE(val == 5);

        ringBuff_ext.Clear();
    }
}
