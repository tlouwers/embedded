
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

// Helper method to add a block of elements to buffer
static bool AddBlock(int index_start, size_t block_size)
{
    bool result = true;
    int* data   = nullptr;
    size_t size = block_size;

    result &= ringBuff_ext.Poke(data, size);
    REQUIRE(result == true);

    if (result)
    {
        // Fill the buffer with 'known' values
        for (size_t i = 0; i < block_size; i++)
        {
            data[i] = index_start++;
        }

        size = block_size;
        result &= ringBuff_ext.Write(size);
        REQUIRE(result == true);
    }

    return result;
}

// Helper method to remove a block of elements
static bool RemoveBlock(int index_start, size_t block_size)
{
    bool result = true;
    int* data   = nullptr;
    size_t size = block_size;

    result &= ringBuff_ext.Peek(data, size);
    REQUIRE(result == true);

    if (result)
    {
        // Empty the buffer with 'known' values
        for (size_t i = 0; i < block_size; i++)
        {
            REQUIRE(data[i] == index_start);
            index_start++;
        }

        size = block_size;
        result &= ringBuff_ext.Read(size);
        REQUIRE(result == true);
    }

    return result;
}


TEST_CASE( "ContiguousRingbuffer Historical Issues", "[ContiguousRingbuffer]" )
{
    // For each section, ContiguousBuffer ringBuff is anew:
    ContiguousRingbuffer<int> ringBuff;

    int* data;
    size_t size = 0;

    REQUIRE(ringBuff.Resize(10) == true);
    REQUIRE(ringBuff.Size() == 0);


    SECTION( "indicate first filled elements at the end, then at the start" )
    {
        REQUIRE(ringBuff_ext.Resize(8) == true);
        REQUIRE(ringBuff_ext.Size() == 0);

        // -----

        int* data;
        size_t size = 0;
        int val = -1;

        ringBuff_ext.SetState(5, 5, 9);                     // Set mWrite(5), mRead(5), mWrap(9) - 4 elements available at end, 4 elements available at start

        REQUIRE(AddOne(6) == true);                         // 1 filled, 3 available at end
        REQUIRE(AddOne(7) == true);                         // 2 filled, 2 available at end
        REQUIRE(ringBuff_ext.CheckState(7, 5, 9) == true);
        REQUIRE(ringBuff_ext.Size() == 2);

        size = 2;
        REQUIRE(ringBuff_ext.Poke(data, size) == true);     // 2 elements available at end
        REQUIRE(size == 2);

        size = 3;
        REQUIRE(ringBuff_ext.Poke(data, size) == true);     // Larger than 2, thus 4 elements available at start
        REQUIRE(size == 4);

        size = 5;
        REQUIRE(ringBuff_ext.Poke(data, size) == false);    // Although 6 available, only 2 or 4 contiguous
        REQUIRE(size == 0);

        size = 4;
        REQUIRE(ringBuff_ext.Poke(data, size) == true);     // Largest contiguous block is 4
        REQUIRE(size == 4);
        data[0] = 1;
        data[1] = 2;
        data[2] = 3;
        REQUIRE(ringBuff_ext.Write(3) == true);
        REQUIRE(ringBuff_ext.CheckState(3, 5, 7) == true);
        REQUIRE(ringBuff_ext.Size() == 5);

        size = 2;
        REQUIRE(ringBuff_ext.Peek(data, size) == true);     // 2 filled at end, 3 at start
        REQUIRE(size == 2);

        size = 3;
        REQUIRE(ringBuff_ext.Peek(data, size) == false);    // Largest filled block is 2 at the end
        REQUIRE(size == 0);

        REQUIRE(RemoveOne(val) == true);                // Remove 2 at the end, 3 at the start remaining
        REQUIRE(val == 6);
        REQUIRE(RemoveOne(val) == true);
        REQUIRE(val == 7);
        REQUIRE(ringBuff_ext.CheckState(3, 0, 9) == true);
        REQUIRE(ringBuff_ext.Size() == 3);

        size = 3;
        REQUIRE(ringBuff_ext.Peek(data, size) == true);
        REQUIRE(size == 3);
        REQUIRE(data[0] == 1);
        REQUIRE(data[1] == 2);
        REQUIRE(data[2] == 3);
        REQUIRE(ringBuff_ext.Read(3) == true);
        REQUIRE(ringBuff_ext.CheckState(3, 3, 9) == true);
        REQUIRE(ringBuff_ext.Size() == 0);
    }

    SECTION( "indicate space available in various conditions" )
    {
        REQUIRE(ringBuff.Resize(4) == true);
        REQUIRE(ringBuff.Size() == 0);

        // -----

        ringBuff.SetState(4, 0, 5);     // Buffer full

        size = 1;
        REQUIRE(ringBuff.Poke(data, size) == false);
        REQUIRE(size == 0);
        REQUIRE(ringBuff.Size() == 4);

        ringBuff.SetState(3, 0, 5);     // Space for 1 element available

        size = 1;
        REQUIRE(ringBuff.Poke(data, size) == true);
        REQUIRE(size == 1);
        REQUIRE(ringBuff.Size() == 3);

        ringBuff.SetState(0, 0, 5);     // Buffer empty

        size = 1;
        REQUIRE(ringBuff.Poke(data, size) == true);
        REQUIRE(size == 4);
        REQUIRE(ringBuff.Size() == 0);

        ringBuff.SetState(4, 4, 5);     // Buffer empty, 1 element available at end, 3 at start

        size = 1;
        REQUIRE(ringBuff.Poke(data, size) == true);
        REQUIRE(size == 1);
        REQUIRE(ringBuff.Size() == 0);

        size = 2;
        REQUIRE(ringBuff.Poke(data, size) == true);
        REQUIRE(size == 3);
        REQUIRE(ringBuff.Size() == 0);

        ringBuff.SetState(0, 4, 5);     // Space for 3 elements available at start

        size = 1;
        REQUIRE(ringBuff.Poke(data, size) == true);
        REQUIRE(size == 3);
        REQUIRE(ringBuff.Size() == 1);
    }

    SECTION( "filling the buffer, read/write shifted to end" )
    {
        REQUIRE(ringBuff.Resize(4) == true);
        REQUIRE(ringBuff.Size() == 0);

        // -----

        ringBuff.SetState(0, 0, 5);                     // Set mWrite(0), mRead(0), mWrap(5) - 4 elements available

        size = 1;
        REQUIRE(ringBuff.Poke(data, size) == true);
        REQUIRE(size == 4);
        REQUIRE(ringBuff.Size() == 0);

        ringBuff.SetState(1, 0, 5);                     // 1 element filled at start, 3 available

        size = 1;
        REQUIRE(ringBuff.Poke(data, size) == true);
        REQUIRE(size == 3);
        REQUIRE(ringBuff.Size() == 1);

        ringBuff.SetState(3, 0, 5);                     // 3 elements filled at start, 1 available

        size = 1;
        REQUIRE(ringBuff.Poke(data, size) == true);
        REQUIRE(size == 1);
        REQUIRE(ringBuff.Size() == 3);


        ringBuff.SetState(4, 0, 5);                     // Buffer full

        size = 1;
        REQUIRE(ringBuff.Poke(data, size) == false);
        REQUIRE(size == 0);
        REQUIRE(ringBuff.Size() == 4);

        ringBuff.SetState(5, 1, 5);                     // Buffer full

        size = 1;
        REQUIRE(ringBuff.Poke(data, size) == false);
        REQUIRE(size == 0);
        REQUIRE(ringBuff.Size() == 4);
    }

    SECTION( "peek generated false positive" )
    {
        REQUIRE(ringBuff_ext.Resize(1024) == true);
        REQUIRE(ringBuff_ext.Size() == 0);

        // -----

        REQUIRE(AddBlock(1, 1020) == true);                 // Add 1020 items, leave 4 remaining
        REQUIRE(ringBuff_ext.CheckState(1020, 0, 1025) == true);

        size = 1;
        REQUIRE(ringBuff_ext.Poke(data, size) == true);     // 4 elements available at end
        REQUIRE(size == 4);

        REQUIRE(AddBlock(1021, 4) == true);                 // Fill the remaining 4 elements
        REQUIRE(ringBuff_ext.CheckState(1024, 0, 1025) == true);

        size = 1;
        REQUIRE(ringBuff_ext.Poke(data, size) == false);    // Buffer full
        REQUIRE(size == 0);
        REQUIRE(ringBuff_ext.Size() == 1024);

        REQUIRE(RemoveBlock(1, 1024) == true);              // Empty the buffer
        REQUIRE(ringBuff_ext.CheckState(1024, 1024, 1025) == true);
        REQUIRE(ringBuff_ext.Size() == 0);

        // --

        ringBuff_ext.SetState(1023, 1023, 1025);            // Buffer empty

        size = 1;
        REQUIRE(ringBuff_ext.Peek(data, size) == false);
        REQUIRE(size == 0);
        REQUIRE(ringBuff_ext.Size() == 0);

        ringBuff_ext.SetState(1024, 1024, 1025);            // Buffer empty

        size = 1;
        REQUIRE(ringBuff_ext.Peek(data, size) == false);
        REQUIRE(size == 0);
        REQUIRE(ringBuff_ext.Size() == 0);

        ringBuff_ext.SetState(0, 0, 1025);

        size = 1;
        REQUIRE(ringBuff_ext.Peek(data, size) == false);    // Empty buffer
        REQUIRE(size == 0);
        REQUIRE(ringBuff_ext.Size() == 0);
    }
}
