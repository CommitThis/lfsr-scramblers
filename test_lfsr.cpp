#include <lfsr.hpp>

#include <test_detail.hpp>

#include <gtest/gtest.h>

#include <format>
#include <set>
#include <cmath>


using tap_lists = ::testing::Types<
        detail::taps_0_1_2,
        detail::taps_0_2_3,
        detail::taps_0_3_4,
        detail::taps_0_3_5,
        detail::taps_0_5_6,
        detail::taps_0_6_7,
        detail::taps_0_4_5_6_8,
        detail::taps_0_5_9,
        detail::taps_0_7_10,
        detail::taps_0_9_11,
        detail::taps_0_4_10_11_12,
        detail::taps_0_8_11_12_13,
        detail::taps_0_2_12_13_14,
        detail::taps_0_14_15,
        detail::taps_0_4_13_15_16,
        detail::taps_0_14_17,
        detail::taps_0_11_18,
        detail::taps_0_14_17_18_19,
        detail::taps_0_17_20,
        detail::taps_0_19_21,
        detail::taps_0_21_22,
        detail::taps_0_18_23,
        detail::taps_0_17_20_22_23_24
    >;




template <typename TapList>
struct LFSRFibonacciBulk : public testing::Test {};


template <typename TapList>
struct LFSRFibonacci : public testing::Test {};


template <typename TapList>
struct LFSRGalois : public testing::Test {};



TYPED_TEST_SUITE(LFSRFibonacciBulk, 
        tap_lists, 
        detail::NameGenerator<"feedthrough_fibonacci_bulk">);


TYPED_TEST_SUITE(LFSRFibonacci,     
        tap_lists, 
        detail::NameGenerator<"feedthrough_fibonacci">);

TYPED_TEST_SUITE(LFSRGalois,     
        tap_lists, 
        detail::NameGenerator<"feedthrough_galois">);



TYPED_TEST(LFSRFibonacciBulk, IsMaximalLength)
{
    using lfsr_type = detail::feedthrough_fibonacci_bulk_from_list_t<TypeParam>;

    auto lfsr = lfsr_type{};
    auto period = 0;
    auto limit = lfsr_type::period * 2;
    auto expected_period = lfsr_type::period;
    auto expected_state = lfsr.state();

    while (period++ < limit)
    {
        lfsr.scramble_bit(0);
        if (lfsr.state() == expected_state) {
            break;
        }
    }

    EXPECT_EQ(period, expected_period);
}



TYPED_TEST(LFSRFibonacciBulk, ScrambledThenDescrambledEqualsInput)
{
    using lfsr_type = detail::feedthrough_fibonacci_bulk_from_list_t<TypeParam>;

    auto scrambler = lfsr_type{};
    auto descrambler = lfsr_type{};

    auto to_scramble = std::vector<std::uint8_t>{
        0x00, 0x01, 0x02, 0x03,
        0x04, 0x05, 0x06, 0x07,
    };

    for (auto b : to_scramble) {
        auto scrambled = scrambler.scramble_byte(b);
        auto descrambled = descrambler.descramble_byte(scrambled);
        ASSERT_EQ(b, descrambled)
            << std::format("0x{:02x} -> 0x{:02x} -> 0x{:02x}", b, scrambled, descrambled);
    }
}


TYPED_TEST(LFSRFibonacciBulk, ScrambledDoesNotEqualInput)
{
    using lfsr_type = detail::feedthrough_fibonacci_bulk_from_list_t<TypeParam>;
    using tap_list = typename lfsr_type::traits::tap_list;

    auto scrambler = lfsr_type{};
    auto descrambler = lfsr_type{};

    auto to_scramble = std::vector<std::uint8_t>{
        0x00, 0x01, 0x02, 0x03,
        0x04, 0x05, 0x06, 0x07,
    };

    for (auto ii = 0ul; ii != to_scramble.size(); ++ii) {
        auto b = to_scramble[ii];
        auto scrambled = scrambler.scramble_byte(b);
        if ((ii * 8) < tap_list::lowest()) {
            continue;
        }
        EXPECT_NE(b, scrambled)
            << std::format("0x{:02x} -> 0x{:02x}", b, scrambled);
    }
}


TYPED_TEST(LFSRFibonacciBulk, ScrambledResultIsSameAsNonBulk)
{
    using lfsr_type      = detail::feedthrough_fibonacci_from_list_t<TypeParam>;
    using lfsr_bulk_type = detail::feedthrough_fibonacci_bulk_from_list_t<TypeParam>;

    auto scrambler      = lfsr_type{};
    auto bulk_scrambler = lfsr_type{};

    auto to_scramble = std::vector<std::uint8_t>{
        0x00, 0x01, 0x02, 0x03,
        0x04, 0x05, 0x06, 0x07,
    };

    for (auto b : to_scramble) {
        auto scrambled      = scrambler.scramble_byte(b);
        auto bulk_scrambled = bulk_scrambler.scramble_byte(b);
        ASSERT_EQ(scrambled, bulk_scrambled)
            << std::format("0x{:02x} (-> 0x{:02x}) (-> 0x{:02x})", b, scrambled, bulk_scrambled);
    }
}


TYPED_TEST(LFSRFibonacci, IsMaximalLength)
{
    using lfsr_type = detail::feedthrough_fibonacci_from_list_t<TypeParam>;

    auto lfsr = lfsr_type{};
    auto period = 0;
    auto limit = lfsr_type::period * 2;
    auto expected_period = lfsr_type::period;
    auto expected_state = lfsr.state();

    while (period++ < limit)
    {
        lfsr.scramble_bit(0);
        if (lfsr.state() == expected_state) {
            break;
        }
    }

    EXPECT_EQ(period, expected_period);
}



TYPED_TEST(LFSRFibonacci, ScrambledThenDescrambledEqualsInput)
{
    using lfsr_type = detail::feedthrough_fibonacci_from_list_t<TypeParam>;

    auto scrambler = lfsr_type{};
    auto descrambler = lfsr_type{};

    auto to_scramble = std::vector<std::uint8_t>{
        0x00, 0x01, 0x02, 0x03,
        0x04, 0x05, 0x06, 0x07,
    };

    for (auto b : to_scramble) {
        auto scrambled = scrambler.scramble_byte(b);
        auto descrambled = descrambler.descramble_byte(scrambled);
        ASSERT_EQ(b, descrambled)
            << std::format("0x{:02x} -> 0x{:02x} -> 0x{:02x}", b, scrambled, descrambled);
    }
}


TYPED_TEST(LFSRFibonacci, ScrambledDoesNotEqualInput)
{
    using lfsr_type = detail::feedthrough_fibonacci_from_list_t<TypeParam>;
    using tap_list = typename lfsr_type::traits::tap_list;

    auto scrambler = lfsr_type{};
    auto descrambler = lfsr_type{};

    auto to_scramble = std::vector<std::uint8_t>{
        0x00, 0x01, 0x02, 0x03,
        0x04, 0x05, 0x06, 0x07,
    };

    for (auto ii = 0ul; ii != to_scramble.size(); ++ii) {
        auto b = to_scramble[ii];
        auto scrambled = scrambler.scramble_byte(b);
        if ((ii * 8) < tap_list::lowest()) {
            continue;
        }
        EXPECT_NE(b, scrambled)
            << std::format("0x{:02x} -> 0x{:02x}", b, scrambled);
    }
}



TYPED_TEST(LFSRGalois, IsMaximalLength)
{
    using lfsr_type = detail::feedthrough_galois_from_list_t<TypeParam>;

    auto lfsr = lfsr_type{};
    auto period = 0;
    auto limit = lfsr_type::period * 2;
    auto expected_period = lfsr_type::period;
    auto expected_state = lfsr.state();

    while (period++ < limit)
    {
        lfsr.scramble_bit(0);
        if (lfsr.state() == expected_state) {
            break;
        }
    }

    EXPECT_EQ(period, expected_period);
}



TYPED_TEST(LFSRGalois, ScrambledThenDescrambledEqualsInput)
{
    using lfsr_type = detail::feedthrough_galois_from_list_t<TypeParam>;

    auto scrambler = lfsr_type{};
    auto descrambler = lfsr_type{};

    auto to_scramble = std::vector<std::uint8_t>{
        0x00, 0x01, 0x02, 0x03,
        0x04, 0x05, 0x06, 0x07,
    };

    for (auto b : to_scramble) {
        auto scrambled = scrambler.scramble_byte(b);
        auto descrambled = descrambler.descramble_byte(scrambled);
        ASSERT_EQ(b, descrambled)
            << std::format("0x{:02x} -> 0x{:02x} -> 0x{:02x}", b, scrambled, descrambled);
    }
}


TYPED_TEST(LFSRGalois, ScrambledDoesNotEqualInput)
{
    using lfsr_type = detail::feedthrough_galois_from_list_t<TypeParam>;

    auto scrambler = lfsr_type{};
    auto descrambler = lfsr_type{};

    auto to_scramble = std::vector<std::uint8_t>{
        0x00, 0x01, 0x02, 0x03,
        0x04, 0x05, 0x06, 0x07,
    };

    for (auto b : to_scramble) {
        auto scrambled = scrambler.scramble_byte(b);
        ASSERT_NE(b, scrambled)
            << std::format("0x{:02x} -> 0x{:02x}", b, scrambled);
    }
}
