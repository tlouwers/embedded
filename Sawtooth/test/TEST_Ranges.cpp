#include <gtest/gtest.h>
#include <cstdint>
#include "../Sawtooth.hpp"

class SawtoothRangesTest : public ::testing::Test {
protected:
    static constexpr uint8_t ITER_COUNT = 10;
    static constexpr uint16_t ref_values1[ITER_COUNT] = { 6, 12, 18, 24, 30, 36, 42,  0,  6, 12 };
    static constexpr uint16_t ref_values2[ITER_COUNT] = { 7, 14, 21, 28, 35,  0,  7, 14, 21, 28 };
    static constexpr uint16_t ref_values_invalid[ITER_COUNT] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    static constexpr uint16_t ref_values_extreme1[ITER_COUNT] = { 1, 0, 1, 0, 1, 0, 1, 0, 1, 0 };
    static constexpr uint16_t ref_values_extreme2[ITER_COUNT] = { 21845, 43690, 0, 21845, 43690, 0, 21845, 43690, 0, 21845 };
};

TEST_F(SawtoothRangesTest, NormalRanges) {
    // For each section variables are anew.
    Sawtooth mSawtooth1(48, 8);  // 0..47, 8 steps of 6
    Sawtooth mSawtooth2(42, 6);  // 0..41, 6 steps of 7

    // Normal range - 8 steps of size 6
    for (auto i = 0; i < ITER_COUNT; i++) {
        uint16_t tmp = mSawtooth1.Next();
        EXPECT_EQ(tmp, ref_values1[i]);
    }

    // Normal range - 6 steps of size 7
    for (auto i = 0; i < ITER_COUNT; i++) {
        uint16_t tmp = mSawtooth2.Next();
        EXPECT_EQ(tmp, ref_values2[i]);
    }
}

TEST_F(SawtoothRangesTest, InvalidRanges) {
    // For each section variables are anew.
    Sawtooth mSawtooth1(0, 0);  // 0..0, 0 steps of 0
    Sawtooth mSawtooth2(1, 0);  // 0..0, 0 steps of 0
    Sawtooth mSawtooth3(2, 0);  // 0..1, 0 steps of 0
    Sawtooth mSawtooth4(2, 1);  // 0..1, 1 steps of 2

    // Invalid range - 0
    for (auto i = 0; i < ITER_COUNT; i++) {
        uint16_t tmp = mSawtooth1.Next();
        EXPECT_EQ(tmp, ref_values_invalid[i]);
    }

    // Invalid range - 1
    for (auto i = 0; i < ITER_COUNT; i++) {
        uint16_t tmp = mSawtooth2.Next();
        EXPECT_EQ(tmp, ref_values_invalid[i]);
    }

    // Valid range - invalid step size of 0
    for (auto i = 0; i < ITER_COUNT; i++) {
        uint16_t tmp = mSawtooth3.Next();
        EXPECT_EQ(tmp, ref_values_invalid[i]);
    }

    // Step size equals range - always returns 0
    for (auto i = 0; i < ITER_COUNT; i++) {
        uint16_t tmp = mSawtooth4.Next();
        EXPECT_EQ(tmp, ref_values_invalid[i]);
    }
}

TEST_F(SawtoothRangesTest, ExtremeRanges) {
    // For each section variables are anew.
    Sawtooth mSawtooth1(2, 2);      // 0..1, 2 steps of 1
    Sawtooth mSawtooth2(65535, 3);  // 0..65535, 3 steps of 21845

    // Smallest range - smallest step size
    for (auto i = 0; i < ITER_COUNT; i++) {
        uint16_t tmp = mSawtooth1.Next();
        EXPECT_EQ(tmp, ref_values_extreme1[i]);
    }

    // Largest range - large step size
    for (auto i = 0; i < ITER_COUNT; i++) {
        uint16_t tmp = mSawtooth2.Next();
        EXPECT_EQ(tmp, ref_values_extreme2[i]);
    }
}
