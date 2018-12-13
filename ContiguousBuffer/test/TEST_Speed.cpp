
#include "../../Catch/catch.hpp"

#include "ContiguousRingbuffer.hpp"
#include <cstddef>      // size_t
#include <cstdint>      // uint8_t, uint32_t
#include <chrono>


static ContiguousRingbuffer<int> ringBuff_ext;          // Our buffer used for testing


// Helper method to add a block of elements to buffer
static bool AddBlock(int index_start, const size_t block_size)
{
    int* data = nullptr;

    if (ringBuff_ext.Poke(data, block_size))
    {
        // Fill the buffer with 'known' values
        for (size_t i = 0; i < block_size; i++)
        {
            data[i] = index_start++;
        }

        return ringBuff_ext.Write(block_size);
    }
    return false;
}

// Helper method to remove a block of elements
static bool RemoveBlock(int index_start, size_t block_size)
{
    int* data   = nullptr;
    size_t size = block_size;

    if (ringBuff_ext.Peek(data, size))
    {
        // Empty the buffer with 'known' values
        for (size_t i = 1; i < block_size; i++)
        {
            if (data[i] <= data[i-1])
            {
                return false;
            }
        }

        return ringBuff_ext.Read(block_size);
    }
    return false;
}


TEST_CASE( "ContiguousRingbuffer speed check", "[ContiguousRingbuffer]" )
{
    const size_t block_size = 7;
    uint32_t count = 0;
    const uint32_t nrOfRuns = 200000;

    REQUIRE(ringBuff_ext.Resize(40) == true);
    REQUIRE(ringBuff_ext.Size() == 0);


    SECTION( "speed check" )
    {
        auto start = std::chrono::steady_clock::now();

        bool result = true;

        for (uint32_t i = 0; i < nrOfRuns; i++)
        {
            result &= AddBlock(count, block_size);
            result &= RemoveBlock(count, block_size);
            count += block_size;

            if (result == false) { break; }
        }

        auto end = std::chrono::steady_clock::now();

        REQUIRE(result == true);

        #ifndef NDEBUG
        WARN("Using DEBUG build - results are NOT accurate");
        #endif // NDEBUG

        WARN("Duration of speed check in milliseconds: " << (std::chrono::duration<double, std::milli>(end - start).count()) );
    }
}
