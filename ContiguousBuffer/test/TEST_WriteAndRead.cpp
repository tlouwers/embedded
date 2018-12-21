
#include "../../Catch/catch.hpp"

#include "ContiguousRingbuffer.hpp"


static ContiguousRingbuffer<int> ringBuff_ext;


// Helper method to add an element to buffer
static bool AddOne(int val)
{
    bool result = true;
    int* data   = nullptr;
    size_t size = 1;

    result &= ringBuff_ext.Poke(data, size);
    REQUIRE(result == true);

    if (result)
    {
        data[0] = val;

        result &= ringBuff_ext.Write(1);
        REQUIRE(result == true);
    }

    return result;
}

// Helper method to remove an element from buffer
static bool RemoveOne(int& val)
{
    bool result = true;
    int* data   = nullptr;
    size_t size = 1;

    result &= ringBuff_ext.Peek(data, size);
    REQUIRE(result == true);

    if (result)
    {
        val = data[0];

        result &= ringBuff_ext.Read(1);
        REQUIRE(result == true);
    }

    return result;
}


TEST_CASE( "ContiguousRingbuffer write and read operations", "[ContiguousRingbuffer]" )
{
    // For each section, ContiguousBuffer ringBuff is anew:
    ContiguousRingbuffer<int> ringBuff;

    int* data;
    size_t size = 0;
    int val = -1;

    REQUIRE(ringBuff.Resize(10) == true);
    REQUIRE(ringBuff.Size() == 0);


    SECTION( "write and read data - blocksize 1" )
    {
        for (int i = 1; i <= 100; i++)
        {
            // Add block with size 1
            size = 1;
            REQUIRE(ringBuff.Poke(data, size) == true);
            data[0] = i;
            REQUIRE(ringBuff.Write(1) == true);

            size = 1;
            REQUIRE(ringBuff.Peek(data, size) == true);
            REQUIRE(data[0] == i);
            REQUIRE(ringBuff.Read(1) == true);
        }

        REQUIRE(ringBuff.Size() == 0);
    }

    SECTION( "write and read data - blocksize 2" )
    {
        for (int i = 1; i <= 100; i += 2)
        {
            // Add block with size 2
            size = 2;
            REQUIRE(ringBuff.Poke(data, size) == true);
            data[0] = i;
            data[1] = i + 1;
            REQUIRE(ringBuff.Write(2) == true);

            size = 2;
            REQUIRE(ringBuff.Peek(data, size) == true);
            REQUIRE(data[0] == i);
            REQUIRE(data[1] == (i + 1));
            REQUIRE(ringBuff.Read(2) == true);
        }

        REQUIRE(ringBuff.Size() == 0);
    }

    SECTION( "write and read data - blocksize 3" )
    {
        for (int i = 1; i <= 100; i += 3)
        {
            // Add block with size 3
            size = 3;
            REQUIRE(ringBuff.Poke(data, size) == true);
            data[0] = i;
            data[1] = i + 1;
            data[2] = i + 2;
            REQUIRE(ringBuff.Write(3) == true);

            size = 3;
            REQUIRE(ringBuff.Peek(data, size) == true);
            REQUIRE(data[0] == i);
            REQUIRE(data[1] == (i + 1));
            REQUIRE(data[2] == (i + 2));
            REQUIRE(ringBuff.Read(3) == true);
        }

        REQUIRE(ringBuff.Size() == 0);
    }

    SECTION( "write and read data - blocksize 4" )
    {
        for (int i = 1; i <= 100; i += 4)
        {
            // Add block with size 4
            size = 4;
            REQUIRE(ringBuff.Poke(data, size) == true);
            data[0] = i;
            data[1] = i + 1;
            data[2] = i + 2;
            data[3] = i + 3;
            REQUIRE(ringBuff.Write(4) == true);

            size = 4;
            REQUIRE(ringBuff.Peek(data, size) == true);
            REQUIRE(data[0] == i);
            REQUIRE(data[1] == (i + 1));
            REQUIRE(data[2] == (i + 2));
            REQUIRE(data[3] == (i + 3));
            REQUIRE(ringBuff.Read(4) == true);
        }

        REQUIRE(ringBuff.Size() == 0);
    }

    SECTION( "write and read data - deep inspection" )
    {
        REQUIRE(ringBuff_ext.Resize(3) == true);
        REQUIRE(ringBuff_ext.Size() == 0);

        // -----

        int index = 1;
        int val   = -1;

        REQUIRE(ringBuff_ext.CheckState(0, 0, 4) == true);

        REQUIRE(AddOne(index++) == true);
        REQUIRE(ringBuff_ext.CheckState(1, 0, 4) == true);

        REQUIRE(RemoveOne(val) == true);
        REQUIRE(ringBuff_ext.CheckState(1, 1, 4) == true);
        REQUIRE(val == 1);

        REQUIRE(AddOne(index++) == true);
        REQUIRE(ringBuff_ext.CheckState(2, 1, 4) == true);

        REQUIRE(RemoveOne(val) == true);
        REQUIRE(ringBuff_ext.CheckState(2, 2, 4) == true);
        REQUIRE(val == 2);

        REQUIRE(AddOne(index++) == true);
        REQUIRE(ringBuff_ext.CheckState(3, 2, 4) == true);

        REQUIRE(RemoveOne(val) == true);
        REQUIRE(ringBuff_ext.CheckState(3, 3, 4) == true);
        REQUIRE(val == 3);

        REQUIRE(AddOne(index++) == true);
        REQUIRE(ringBuff_ext.CheckState(0, 3, 4) == true);

        REQUIRE(RemoveOne(val) == true);
        REQUIRE(ringBuff_ext.CheckState(0, 0, 4) == true);
        REQUIRE(val == 4);

        REQUIRE(AddOne(index++) == true);
        REQUIRE(ringBuff_ext.CheckState(1, 0, 4) == true);

        REQUIRE(RemoveOne(val) == true);
        REQUIRE(ringBuff_ext.CheckState(1, 1, 4) == true);
        REQUIRE(val == 5);

        ringBuff_ext.Clear();
    }

    SECTION( "write and read data - data must be contiguous" )
    {
        REQUIRE(ringBuff.Resize(3) == true);
        REQUIRE(ringBuff.Size() == 0);

        // -----

        ringBuff.SetState(3, 3, 4);                     // Set mWrite(3), mRead(3), mWrap(4) - 1 element available at end, 2 elements available at start

        size = 1;
        REQUIRE(ringBuff.Poke(data, size) == true);     // 3 elements, 1 at end, other 2 at start not contiguous
        REQUIRE(ringBuff.Write(1) == true);
        REQUIRE(ringBuff.ContiguousSize() == 1);
        REQUIRE(ringBuff.CheckState(0, 3, 4) == true);

        ringBuff.SetState(0, 0, 4);                     // Set mWrite(0), mRead(0), mWrap(4) - 3 elements available at start

        size = 3;
        REQUIRE(ringBuff.Poke(data, size) == true);     // 3 elements available at start
        REQUIRE(ringBuff.Size() == 0);                  // Nothing in buffer yet
        REQUIRE(ringBuff.Write(3) == true);
        REQUIRE(ringBuff.ContiguousSize() == 3);
        REQUIRE(ringBuff.CheckState(3, 0, 4) == true);

        size = 1;
        REQUIRE(ringBuff.Poke(data, size) == false);    // Buffer full: 3 elements placed
        REQUIRE(ringBuff.Size() == 3);

        REQUIRE(ringBuff.Read(1) == true);              // Remove 1 element, 2 elements remain in the middle
        REQUIRE(ringBuff.ContiguousSize() == 2);

        size = 1;
        REQUIRE(ringBuff.Poke(data, size) == true);     // 1 element available at end
        REQUIRE(size == 1);
        REQUIRE(ringBuff.Write(1) == true);
        REQUIRE(ringBuff.ContiguousSize() == 3);
        REQUIRE(ringBuff.CheckState(0, 1, 4) == true);
    }

    SECTION( "write and read data - with wrap" )
    {
        REQUIRE(ringBuff_ext.Resize(5) == true);
        REQUIRE(ringBuff_ext.Size() == 0);

        // -----

        REQUIRE(AddOne(1) == true);         // Add 5 elements
        REQUIRE(ringBuff_ext.Size() == 1);
        REQUIRE(ringBuff_ext.ContiguousSize() == 1);
        REQUIRE(AddOne(2) == true);
        REQUIRE(ringBuff_ext.Size() == 2);
        REQUIRE(ringBuff_ext.ContiguousSize() == 2);
        REQUIRE(AddOne(3) == true);
        REQUIRE(ringBuff_ext.Size() == 3);
        REQUIRE(ringBuff_ext.ContiguousSize() == 3);
        REQUIRE(AddOne(4) == true);
        REQUIRE(ringBuff_ext.Size() == 4);
        REQUIRE(ringBuff_ext.ContiguousSize() == 4);
        REQUIRE(AddOne(5) == true);
        REQUIRE(ringBuff_ext.Size() == 5);
        REQUIRE(ringBuff_ext.ContiguousSize() == 5);

        REQUIRE(RemoveOne(val) == true);    // Remove 3 elements
        REQUIRE(val == 1);
        REQUIRE(ringBuff_ext.Size() == 4);
        REQUIRE(ringBuff_ext.ContiguousSize() == 4);
        REQUIRE(RemoveOne(val) == true);
        REQUIRE(val == 2);
        REQUIRE(ringBuff_ext.Size() == 3);
        REQUIRE(ringBuff_ext.ContiguousSize() == 3);
        REQUIRE(RemoveOne(val) == true);
        REQUIRE(val == 3);
        REQUIRE(ringBuff_ext.Size() == 2);
        REQUIRE(ringBuff_ext.ContiguousSize() == 2);

        REQUIRE(AddOne(6) == true);         // Write wraps, value at end of buffer
        REQUIRE(ringBuff_ext.Size() == 3);
        REQUIRE(ringBuff_ext.ContiguousSize() == 3);
        REQUIRE(ringBuff_ext.CheckState(0, 3, 6) == true);

        REQUIRE(RemoveOne(val) == true);
        REQUIRE(val == 4);
        REQUIRE(ringBuff_ext.Size() == 2);
        REQUIRE(ringBuff_ext.ContiguousSize() == 2);
        REQUIRE(RemoveOne(val) == true);
        REQUIRE(val == 5);
        REQUIRE(ringBuff_ext.Size() == 1);
        REQUIRE(ringBuff_ext.ContiguousSize() == 1);
        REQUIRE(RemoveOne(val) == true);
        REQUIRE(val == 6);
        REQUIRE(ringBuff_ext.Size() == 0);
        REQUIRE(ringBuff_ext.ContiguousSize() == 0);
    }

    SECTION( "regression check: threading issue due to incorrect available space calculation" )
    {
        REQUIRE(ringBuff_ext.Resize(3) == true);
        REQUIRE(ringBuff_ext.Size() == 0);

        // -----

        REQUIRE(AddOne(1) == true);         // Add 3 elements
        REQUIRE(ringBuff_ext.Size() == 1);
        REQUIRE(AddOne(2) == true);
        REQUIRE(ringBuff_ext.Size() == 2);
        REQUIRE(AddOne(3) == true);
        REQUIRE(ringBuff_ext.Size() == 3);

        REQUIRE(RemoveOne(val) == true);    // Remove 2 elements
        REQUIRE(val == 1);
        REQUIRE(ringBuff_ext.Size() == 2);
        REQUIRE(RemoveOne(val) == true);
        REQUIRE(val == 2);
        REQUIRE(ringBuff_ext.Size() == 1);

        REQUIRE(AddOne(4) == true);
        REQUIRE(ringBuff_ext.Size() == 2);

        // Had threading issue here: depending on the value of read the available space is calculated incorrectly.

        size = 1;
        REQUIRE(ringBuff_ext.Poke(data, size) == true);
        REQUIRE(size == 1);
        data[0] = 5;
        REQUIRE(ringBuff_ext.Write(1) == true);
        REQUIRE(ringBuff_ext.CheckState(1, 2, 4) == true);
    }
}
