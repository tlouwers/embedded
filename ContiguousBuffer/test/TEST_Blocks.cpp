
#include "../../Catch/catch.hpp"

#include "ContiguousRingbuffer.hpp"


static ContiguousRingbuffer<int> ringBuff_ext;


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


TEST_CASE( "ContiguousRingbuffer large blocks", "[ContiguousRingbuffer]" )
{
    const size_t block_size = 256;
    int* data;
    size_t size = 0;


    SECTION( "add and remove large blocks - start at start" )
    {
        REQUIRE(ringBuff_ext.Resize(block_size * 4) == true);
        REQUIRE(ringBuff_ext.Size() == 0);

        // -----

        int index = 0;

        REQUIRE(AddBlock(index, block_size) == true);       // Block written at the start, wrap reduced from 1025 to 1024
        index += block_size;                                // 1 block = 256 elements in buffer
        REQUIRE(AddBlock(index, block_size) == true);
        index += block_size;                                // 2 blocks = 512 elements in buffer
        REQUIRE(AddBlock(index, block_size) == true);
        index += block_size;                                // 3 blocks = 768 elements in buffer
        REQUIRE(AddBlock(index, block_size) == true);
        index += block_size;                                // 4 blocks = 1024 elements in buffer

        REQUIRE(ringBuff_ext.CheckState(1024, 0, 1025) == true);   // Wrapped

        size = block_size;
        REQUIRE(ringBuff_ext.Poke(data, size) == false);    // Cannot add another block
        REQUIRE(size == 0);
        REQUIRE(ringBuff_ext.CheckState(1024, 0, 1025) == true);

        size = 1;
        REQUIRE(ringBuff_ext.Poke(data, size) == false);    // Cannot even add a single element
        REQUIRE(size == 0);
        REQUIRE(ringBuff_ext.CheckState(1024, 0, 1025) == true);

        size = block_size;
        REQUIRE(ringBuff_ext.Peek(data, size) == true);     // Elements available from the start, 4 blocks
        REQUIRE(size == 1024);
        REQUIRE(ringBuff_ext.Size() == 1024);

        index = 0;

        REQUIRE(RemoveBlock(index, block_size) == true);    // Read 1 block, 3 remaining
        REQUIRE(ringBuff_ext.Size() == 768);
        index += block_size;

        REQUIRE(RemoveBlock(index, block_size) == true);    // Read another block, 2 remaining
        REQUIRE(ringBuff_ext.Size() == 512);
        index += block_size;

        REQUIRE(RemoveBlock(index, block_size) == true);    // Read another block, 1 remaining
        REQUIRE(ringBuff_ext.Size() == 256);
        index += block_size;

        REQUIRE(RemoveBlock(index, block_size) == true);    // Buffer empty
        REQUIRE(ringBuff_ext.Size() == 0);
    }

    SECTION( "add and remove large blocks - start at end" )
    {
        REQUIRE(ringBuff_ext.Resize(block_size * 4) == true);
        REQUIRE(ringBuff_ext.Size() == 0);

        // -----

        int index = 0;

        ringBuff_ext.SetState(1024, 1024, 1025);            // Filled 4 blocks, removed 4 blocks

        REQUIRE(AddBlock(index, block_size) == true);       // Block written at the start, wrap reduced from 1025 to 1024
        index += block_size;                                // 1 block = 256 elements in buffer
        REQUIRE(AddBlock(index, block_size) == true);
        index += block_size;                                // 2 blocks = 512 elements in buffer
        REQUIRE(AddBlock(index, block_size) == true);
        index += block_size;                                // 3 blocks = 768 elements in buffer

        size = block_size;
        REQUIRE(ringBuff_ext.Poke(data, size) == false);    // Cannot add another block
        REQUIRE(size == 0);
        REQUIRE(ringBuff_ext.CheckState(768, 1024, 1024) == true);

        size = block_size;
        REQUIRE(ringBuff_ext.Peek(data, size) == true);     // Elements available from the start, 3 blocks
        REQUIRE(size == 768);
        REQUIRE(ringBuff_ext.Size() == 768);

        index = 0;

        REQUIRE(RemoveBlock(index, block_size) == true);    // Read 1 block, 2 remaining
        index += block_size;

        REQUIRE(ringBuff_ext.CheckState(768, 256, 1025) == true);
        REQUIRE(ringBuff_ext.Size() == 512);                // After read still 2 blocks available

        REQUIRE(RemoveBlock(index, block_size) == true);    // Read another block, 1 remaining
        REQUIRE(ringBuff_ext.Size() == 256);
        index += block_size;

        REQUIRE(RemoveBlock(index, block_size) == true);    // Buffer empty
        REQUIRE(ringBuff_ext.Size() == 0);
    }
}
