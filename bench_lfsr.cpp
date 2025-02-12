#include <lfsr.hpp>

#include <test_detail.hpp>
#include <bench_detail.hpp>

#include <benchmark/benchmark.h>



const auto test_data = std::array<std::uint8_t, 128>{
    0,   1,   2,   3,   4,   5,   6,   7, 
    8,   9,   10,  11,  12,  13,  14,  15,
    16,  17,  18,  19,  20,  21,  22,  23,
    24,  25,  26,  27,  28,  29,  30,  31,
    32,  33,  34,  35,  36,  37,  38,  39,
    40,  41,  42,  43,  44,  45,  46,  47,
    48,  49,  50,  51,  52,  53,  54,  55,
    56,  57,  58,  59,  60,  61,  62,  63,
    64,  65,  66,  67,  68,  69,  70,  71,
    72,  73,  74,  75,  76,  77,  78,  79,
    80,  81,  82,  83,  84,  85,  86,  87,
    88,  89,  90,  91,  92,  93,  94,  95,
    96,  97,  98,  99,  100, 101, 102, 103,
    104, 105, 106, 107, 108, 109, 110, 111,
    112, 113, 114, 115, 116, 117, 118, 119,
    120, 121, 122, 123, 124, 125, 126, 127
};




// template <typename TapList>
// auto LFSR_FeedthroughGalois(benchmark::State & state)
// {
//     using lfsr_type = detail::feedthrough_galois_from_list_t<TapList>;

//     auto lfsr = lfsr_type{};
//     for (auto _ : state) {
//         auto x = lfsr.scramble_byte(42);
//         benchmark::DoNotOptimize(x);
//     }
// }

// template <typename TapList>
// auto LFSR_FeedthroughFibonacci(benchmark::State & state)
// {
//     using lfsr_type = detail::feedthrough_fibonacci_from_list_t<TapList>;

//     auto lfsr = lfsr_type{};
//     for (auto _ : state) {
//         auto x = lfsr.scramble_byte(42);
//         benchmark::DoNotOptimize(x);
//     }
// }

// template <typename TapList>
// auto LFSR_FeedthroughFibonacciBulk(benchmark::State & state)
// {
//     using lfsr_type = detail::feedthrough_fibonacci_bulk_from_list_t<TapList>;

//     auto lfsr = lfsr_type{};
//     for (auto _ : state) {
//         auto x = lfsr.scramble_byte(42);
//         benchmark::DoNotOptimize(x);
//     }
// }

auto make_all_zeroes() -> std::vector<std::uint8_t>
{
    auto result = std::vector<std::uint8_t>{};
    for (auto ii = 0; ii != 128; ++ii) {
        result.push_back(0);
    }
    return result;
}

auto all_zeroes() -> std::vector<std::uint8_t> const &
{
    static auto result = make_all_zeroes();
    return result;
}

template <typename TapList>
auto LFSR_FeedthroughGalois(benchmark::State & state)
{
    using lfsr_type = detail::feedthrough_galois_from_list_t<TapList>;

    auto result = std::vector<std::uint8_t>();
    auto lfsr = lfsr_type{};
    for (auto _ : state) {
        state.PauseTiming();
        result.clear();
        result.reserve(test_data.size());
        state.ResumeTiming();
        lfsr.scramble_range(std::begin(test_data), 
                std::end(test_data), 
                std::back_inserter(result));
        benchmark::DoNotOptimize(result);
    }
    if (result.size() != 128) {
        throw std::runtime_error("result.size() != 128");
    }
    if (result == all_zeroes()) {
        throw std::runtime_error("result == all_zeroes()");
    }
}


template <typename TapList>
auto LFSR_FeedthroughFibonacci(benchmark::State & state)
{
    using lfsr_type = detail::feedthrough_fibonacci_from_list_t<TapList>;

    auto result = std::vector<std::uint8_t>();
    auto lfsr = lfsr_type{};
    for (auto _ : state) {
        state.PauseTiming();
        result.clear();
        result.reserve(test_data.size());
        state.ResumeTiming();
        lfsr.scramble_range(std::begin(test_data), 
                std::end(test_data), 
                std::back_inserter(result));
        benchmark::DoNotOptimize(result);
    }
    if (result.size() != 128) {
        throw std::runtime_error("result.size() != 128");
    }
    if (result == all_zeroes()) {
        throw std::runtime_error("result == all_zeroes()");
    }
}


template <typename TapList>
auto LFSR_FeedthroughFibonacciBulk(benchmark::State & state)
{
    using lfsr_type = detail::feedthrough_fibonacci_bulk_from_list_t<TapList>;

    auto result = std::vector<std::uint8_t>();
    auto lfsr = lfsr_type{};
    for (auto _ : state) {
        state.PauseTiming();
        result.clear();
        result.reserve(test_data.size());
        state.ResumeTiming();
        lfsr.scramble_range(std::begin(test_data), 
                std::end(test_data), 
                std::back_inserter(result));
        benchmark::DoNotOptimize(result);
    }
    if (result.size() != 128) {
        throw std::runtime_error("result.size() != 128");
    }
    if (result == all_zeroes()) {
        throw std::runtime_error("result == all_zeroes()");
    }
}

// #define MIN_TIME ->MinTime(0.1)
#define MIN_TIME

BENCHMARK_TEMPLATE(LFSR_FeedthroughGalois, Degree_5)MIN_TIME;
BENCHMARK_TEMPLATE(LFSR_FeedthroughGalois, Degree_6)MIN_TIME;
BENCHMARK_TEMPLATE(LFSR_FeedthroughGalois, Degree_7)MIN_TIME;
BENCHMARK_TEMPLATE(LFSR_FeedthroughGalois, Degree_8)MIN_TIME;
BENCHMARK_TEMPLATE(LFSR_FeedthroughGalois, Degree_9)MIN_TIME;
BENCHMARK_TEMPLATE(LFSR_FeedthroughGalois, Degree_10)MIN_TIME;
BENCHMARK_TEMPLATE(LFSR_FeedthroughGalois, Degree_11)MIN_TIME;
BENCHMARK_TEMPLATE(LFSR_FeedthroughGalois, Degree_12)MIN_TIME;
BENCHMARK_TEMPLATE(LFSR_FeedthroughGalois, Degree_13)MIN_TIME;
BENCHMARK_TEMPLATE(LFSR_FeedthroughGalois, Degree_14)MIN_TIME;
BENCHMARK_TEMPLATE(LFSR_FeedthroughGalois, Degree_15)MIN_TIME;
BENCHMARK_TEMPLATE(LFSR_FeedthroughGalois, Degree_16)MIN_TIME;
BENCHMARK_TEMPLATE(LFSR_FeedthroughGalois, Degree_17)MIN_TIME;
BENCHMARK_TEMPLATE(LFSR_FeedthroughGalois, Degree_18)MIN_TIME;
BENCHMARK_TEMPLATE(LFSR_FeedthroughGalois, Degree_19)MIN_TIME;
BENCHMARK_TEMPLATE(LFSR_FeedthroughGalois, Degree_20)MIN_TIME;
BENCHMARK_TEMPLATE(LFSR_FeedthroughGalois, Degree_21)MIN_TIME;
BENCHMARK_TEMPLATE(LFSR_FeedthroughGalois, Degree_22)MIN_TIME;
BENCHMARK_TEMPLATE(LFSR_FeedthroughGalois, Degree_23)MIN_TIME;
BENCHMARK_TEMPLATE(LFSR_FeedthroughGalois, Degree_24)MIN_TIME;
BENCHMARK_TEMPLATE(LFSR_FeedthroughGalois, Degree_25)MIN_TIME;
BENCHMARK_TEMPLATE(LFSR_FeedthroughGalois, Degree_26)MIN_TIME;
BENCHMARK_TEMPLATE(LFSR_FeedthroughGalois, Degree_27)MIN_TIME;
BENCHMARK_TEMPLATE(LFSR_FeedthroughGalois, Degree_28)MIN_TIME;
BENCHMARK_TEMPLATE(LFSR_FeedthroughGalois, Degree_29)MIN_TIME;
BENCHMARK_TEMPLATE(LFSR_FeedthroughGalois, Degree_30)MIN_TIME;
BENCHMARK_TEMPLATE(LFSR_FeedthroughGalois, Degree_31)MIN_TIME;
BENCHMARK_TEMPLATE(LFSR_FeedthroughGalois, Degree_32)MIN_TIME;
BENCHMARK_TEMPLATE(LFSR_FeedthroughGalois, Degree_33)MIN_TIME;
BENCHMARK_TEMPLATE(LFSR_FeedthroughGalois, Degree_34)MIN_TIME;
BENCHMARK_TEMPLATE(LFSR_FeedthroughGalois, Degree_35)MIN_TIME;
BENCHMARK_TEMPLATE(LFSR_FeedthroughGalois, Degree_36)MIN_TIME;
BENCHMARK_TEMPLATE(LFSR_FeedthroughGalois, Degree_37)MIN_TIME;
BENCHMARK_TEMPLATE(LFSR_FeedthroughGalois, Degree_38)MIN_TIME;
BENCHMARK_TEMPLATE(LFSR_FeedthroughGalois, Degree_39)MIN_TIME;
BENCHMARK_TEMPLATE(LFSR_FeedthroughGalois, Degree_40)MIN_TIME;
BENCHMARK_TEMPLATE(LFSR_FeedthroughGalois, Degree_41)MIN_TIME;
BENCHMARK_TEMPLATE(LFSR_FeedthroughGalois, Degree_42)MIN_TIME;
BENCHMARK_TEMPLATE(LFSR_FeedthroughGalois, Degree_43)MIN_TIME;
BENCHMARK_TEMPLATE(LFSR_FeedthroughGalois, Degree_44)MIN_TIME;
BENCHMARK_TEMPLATE(LFSR_FeedthroughGalois, Degree_45)MIN_TIME;
BENCHMARK_TEMPLATE(LFSR_FeedthroughGalois, Degree_46)MIN_TIME;
BENCHMARK_TEMPLATE(LFSR_FeedthroughGalois, Degree_47)MIN_TIME;
BENCHMARK_TEMPLATE(LFSR_FeedthroughGalois, Degree_48)MIN_TIME;
BENCHMARK_TEMPLATE(LFSR_FeedthroughGalois, Degree_49)MIN_TIME;
BENCHMARK_TEMPLATE(LFSR_FeedthroughGalois, Degree_50)MIN_TIME;
BENCHMARK_TEMPLATE(LFSR_FeedthroughGalois, Degree_51)MIN_TIME;
BENCHMARK_TEMPLATE(LFSR_FeedthroughGalois, Degree_52)MIN_TIME;
BENCHMARK_TEMPLATE(LFSR_FeedthroughGalois, Degree_53)MIN_TIME;
BENCHMARK_TEMPLATE(LFSR_FeedthroughGalois, Degree_54)MIN_TIME;
BENCHMARK_TEMPLATE(LFSR_FeedthroughGalois, Degree_55)MIN_TIME;
BENCHMARK_TEMPLATE(LFSR_FeedthroughGalois, Degree_56)MIN_TIME;
BENCHMARK_TEMPLATE(LFSR_FeedthroughGalois, Degree_57)MIN_TIME;
BENCHMARK_TEMPLATE(LFSR_FeedthroughGalois, Degree_58)MIN_TIME;
BENCHMARK_TEMPLATE(LFSR_FeedthroughGalois, Degree_59)MIN_TIME;
BENCHMARK_TEMPLATE(LFSR_FeedthroughGalois, Degree_60)MIN_TIME;
BENCHMARK_TEMPLATE(LFSR_FeedthroughGalois, Degree_61)MIN_TIME;
BENCHMARK_TEMPLATE(LFSR_FeedthroughGalois, Degree_62)MIN_TIME;
BENCHMARK_TEMPLATE(LFSR_FeedthroughGalois, Degree_63)MIN_TIME;


BENCHMARK_TEMPLATE(LFSR_FeedthroughFibonacci, Degree_5)MIN_TIME;;
BENCHMARK_TEMPLATE(LFSR_FeedthroughFibonacci, Degree_6)MIN_TIME;
BENCHMARK_TEMPLATE(LFSR_FeedthroughFibonacci, Degree_7)MIN_TIME;
BENCHMARK_TEMPLATE(LFSR_FeedthroughFibonacci, Degree_8)MIN_TIME;
BENCHMARK_TEMPLATE(LFSR_FeedthroughFibonacci, Degree_9)MIN_TIME;
BENCHMARK_TEMPLATE(LFSR_FeedthroughFibonacci, Degree_10)MIN_TIME;
BENCHMARK_TEMPLATE(LFSR_FeedthroughFibonacci, Degree_11)MIN_TIME;
BENCHMARK_TEMPLATE(LFSR_FeedthroughFibonacci, Degree_12)MIN_TIME;
BENCHMARK_TEMPLATE(LFSR_FeedthroughFibonacci, Degree_13)MIN_TIME;
BENCHMARK_TEMPLATE(LFSR_FeedthroughFibonacci, Degree_14)MIN_TIME;
BENCHMARK_TEMPLATE(LFSR_FeedthroughFibonacci, Degree_15)MIN_TIME;
BENCHMARK_TEMPLATE(LFSR_FeedthroughFibonacci, Degree_16)MIN_TIME;
BENCHMARK_TEMPLATE(LFSR_FeedthroughFibonacci, Degree_17)MIN_TIME;
BENCHMARK_TEMPLATE(LFSR_FeedthroughFibonacci, Degree_18)MIN_TIME;
BENCHMARK_TEMPLATE(LFSR_FeedthroughFibonacci, Degree_19)MIN_TIME;
BENCHMARK_TEMPLATE(LFSR_FeedthroughFibonacci, Degree_20)MIN_TIME;
BENCHMARK_TEMPLATE(LFSR_FeedthroughFibonacci, Degree_21)MIN_TIME;
BENCHMARK_TEMPLATE(LFSR_FeedthroughFibonacci, Degree_22)MIN_TIME;
BENCHMARK_TEMPLATE(LFSR_FeedthroughFibonacci, Degree_23)MIN_TIME;
BENCHMARK_TEMPLATE(LFSR_FeedthroughFibonacci, Degree_24)MIN_TIME;
BENCHMARK_TEMPLATE(LFSR_FeedthroughFibonacci, Degree_25)MIN_TIME;
BENCHMARK_TEMPLATE(LFSR_FeedthroughFibonacci, Degree_26)MIN_TIME;
BENCHMARK_TEMPLATE(LFSR_FeedthroughFibonacci, Degree_27)MIN_TIME;
BENCHMARK_TEMPLATE(LFSR_FeedthroughFibonacci, Degree_28)MIN_TIME;
BENCHMARK_TEMPLATE(LFSR_FeedthroughFibonacci, Degree_29)MIN_TIME;
BENCHMARK_TEMPLATE(LFSR_FeedthroughFibonacci, Degree_30)MIN_TIME;
BENCHMARK_TEMPLATE(LFSR_FeedthroughFibonacci, Degree_31)MIN_TIME;
BENCHMARK_TEMPLATE(LFSR_FeedthroughFibonacci, Degree_32)MIN_TIME;
BENCHMARK_TEMPLATE(LFSR_FeedthroughFibonacci, Degree_33)MIN_TIME;
BENCHMARK_TEMPLATE(LFSR_FeedthroughFibonacci, Degree_34)MIN_TIME;
BENCHMARK_TEMPLATE(LFSR_FeedthroughFibonacci, Degree_35)MIN_TIME;
BENCHMARK_TEMPLATE(LFSR_FeedthroughFibonacci, Degree_36)MIN_TIME;
BENCHMARK_TEMPLATE(LFSR_FeedthroughFibonacci, Degree_37)MIN_TIME;
BENCHMARK_TEMPLATE(LFSR_FeedthroughFibonacci, Degree_38)MIN_TIME;
BENCHMARK_TEMPLATE(LFSR_FeedthroughFibonacci, Degree_39)MIN_TIME;
BENCHMARK_TEMPLATE(LFSR_FeedthroughFibonacci, Degree_40)MIN_TIME;
BENCHMARK_TEMPLATE(LFSR_FeedthroughFibonacci, Degree_41)MIN_TIME;
BENCHMARK_TEMPLATE(LFSR_FeedthroughFibonacci, Degree_42)MIN_TIME;
BENCHMARK_TEMPLATE(LFSR_FeedthroughFibonacci, Degree_43)MIN_TIME;
BENCHMARK_TEMPLATE(LFSR_FeedthroughFibonacci, Degree_44)MIN_TIME;
BENCHMARK_TEMPLATE(LFSR_FeedthroughFibonacci, Degree_45)MIN_TIME;
BENCHMARK_TEMPLATE(LFSR_FeedthroughFibonacci, Degree_46)MIN_TIME;
BENCHMARK_TEMPLATE(LFSR_FeedthroughFibonacci, Degree_47)MIN_TIME;
BENCHMARK_TEMPLATE(LFSR_FeedthroughFibonacci, Degree_48)MIN_TIME;
BENCHMARK_TEMPLATE(LFSR_FeedthroughFibonacci, Degree_49)MIN_TIME;
BENCHMARK_TEMPLATE(LFSR_FeedthroughFibonacci, Degree_50)MIN_TIME;
BENCHMARK_TEMPLATE(LFSR_FeedthroughFibonacci, Degree_51)MIN_TIME;
BENCHMARK_TEMPLATE(LFSR_FeedthroughFibonacci, Degree_52)MIN_TIME;
BENCHMARK_TEMPLATE(LFSR_FeedthroughFibonacci, Degree_53)MIN_TIME;
BENCHMARK_TEMPLATE(LFSR_FeedthroughFibonacci, Degree_54)MIN_TIME;
BENCHMARK_TEMPLATE(LFSR_FeedthroughFibonacci, Degree_55)MIN_TIME;
BENCHMARK_TEMPLATE(LFSR_FeedthroughFibonacci, Degree_56)MIN_TIME;
BENCHMARK_TEMPLATE(LFSR_FeedthroughFibonacci, Degree_57)MIN_TIME;
BENCHMARK_TEMPLATE(LFSR_FeedthroughFibonacci, Degree_58)MIN_TIME;
BENCHMARK_TEMPLATE(LFSR_FeedthroughFibonacci, Degree_59)MIN_TIME;
BENCHMARK_TEMPLATE(LFSR_FeedthroughFibonacci, Degree_60)MIN_TIME;
BENCHMARK_TEMPLATE(LFSR_FeedthroughFibonacci, Degree_61)MIN_TIME;
BENCHMARK_TEMPLATE(LFSR_FeedthroughFibonacci, Degree_62)MIN_TIME;
BENCHMARK_TEMPLATE(LFSR_FeedthroughFibonacci, Degree_63)MIN_TIME;

BENCHMARK_TEMPLATE(LFSR_FeedthroughFibonacciBulk, Degree_5)MIN_TIME;;
BENCHMARK_TEMPLATE(LFSR_FeedthroughFibonacciBulk, Degree_6)MIN_TIME;
BENCHMARK_TEMPLATE(LFSR_FeedthroughFibonacciBulk, Degree_7)MIN_TIME;
BENCHMARK_TEMPLATE(LFSR_FeedthroughFibonacciBulk, Degree_8)MIN_TIME;
BENCHMARK_TEMPLATE(LFSR_FeedthroughFibonacciBulk, Degree_9)MIN_TIME;
BENCHMARK_TEMPLATE(LFSR_FeedthroughFibonacciBulk, Degree_10)MIN_TIME;
BENCHMARK_TEMPLATE(LFSR_FeedthroughFibonacciBulk, Degree_11)MIN_TIME;
BENCHMARK_TEMPLATE(LFSR_FeedthroughFibonacciBulk, Degree_12)MIN_TIME;
BENCHMARK_TEMPLATE(LFSR_FeedthroughFibonacciBulk, Degree_13)MIN_TIME;
BENCHMARK_TEMPLATE(LFSR_FeedthroughFibonacciBulk, Degree_14)MIN_TIME;
BENCHMARK_TEMPLATE(LFSR_FeedthroughFibonacciBulk, Degree_15)MIN_TIME;
BENCHMARK_TEMPLATE(LFSR_FeedthroughFibonacciBulk, Degree_16)MIN_TIME;
BENCHMARK_TEMPLATE(LFSR_FeedthroughFibonacciBulk, Degree_17)MIN_TIME;
BENCHMARK_TEMPLATE(LFSR_FeedthroughFibonacciBulk, Degree_18)MIN_TIME;
BENCHMARK_TEMPLATE(LFSR_FeedthroughFibonacciBulk, Degree_19)MIN_TIME;
BENCHMARK_TEMPLATE(LFSR_FeedthroughFibonacciBulk, Degree_20)MIN_TIME;
BENCHMARK_TEMPLATE(LFSR_FeedthroughFibonacciBulk, Degree_21)MIN_TIME;
BENCHMARK_TEMPLATE(LFSR_FeedthroughFibonacciBulk, Degree_22)MIN_TIME;
BENCHMARK_TEMPLATE(LFSR_FeedthroughFibonacciBulk, Degree_23)MIN_TIME;
BENCHMARK_TEMPLATE(LFSR_FeedthroughFibonacciBulk, Degree_24)MIN_TIME;
BENCHMARK_TEMPLATE(LFSR_FeedthroughFibonacciBulk, Degree_25)MIN_TIME;
BENCHMARK_TEMPLATE(LFSR_FeedthroughFibonacciBulk, Degree_26)MIN_TIME;
BENCHMARK_TEMPLATE(LFSR_FeedthroughFibonacciBulk, Degree_27)MIN_TIME;
BENCHMARK_TEMPLATE(LFSR_FeedthroughFibonacciBulk, Degree_28)MIN_TIME;
BENCHMARK_TEMPLATE(LFSR_FeedthroughFibonacciBulk, Degree_29)MIN_TIME;
BENCHMARK_TEMPLATE(LFSR_FeedthroughFibonacciBulk, Degree_30)MIN_TIME;
BENCHMARK_TEMPLATE(LFSR_FeedthroughFibonacciBulk, Degree_31)MIN_TIME;
BENCHMARK_TEMPLATE(LFSR_FeedthroughFibonacciBulk, Degree_32)MIN_TIME;
BENCHMARK_TEMPLATE(LFSR_FeedthroughFibonacciBulk, Degree_33)MIN_TIME;
BENCHMARK_TEMPLATE(LFSR_FeedthroughFibonacciBulk, Degree_34)MIN_TIME;
BENCHMARK_TEMPLATE(LFSR_FeedthroughFibonacciBulk, Degree_35)MIN_TIME;
BENCHMARK_TEMPLATE(LFSR_FeedthroughFibonacciBulk, Degree_36)MIN_TIME;
BENCHMARK_TEMPLATE(LFSR_FeedthroughFibonacciBulk, Degree_37)MIN_TIME;
BENCHMARK_TEMPLATE(LFSR_FeedthroughFibonacciBulk, Degree_38)MIN_TIME;
BENCHMARK_TEMPLATE(LFSR_FeedthroughFibonacciBulk, Degree_39)MIN_TIME;
BENCHMARK_TEMPLATE(LFSR_FeedthroughFibonacciBulk, Degree_40)MIN_TIME;
BENCHMARK_TEMPLATE(LFSR_FeedthroughFibonacciBulk, Degree_41)MIN_TIME;
BENCHMARK_TEMPLATE(LFSR_FeedthroughFibonacciBulk, Degree_42)MIN_TIME;
BENCHMARK_TEMPLATE(LFSR_FeedthroughFibonacciBulk, Degree_43)MIN_TIME;
BENCHMARK_TEMPLATE(LFSR_FeedthroughFibonacciBulk, Degree_44)MIN_TIME;
BENCHMARK_TEMPLATE(LFSR_FeedthroughFibonacciBulk, Degree_45)MIN_TIME;
BENCHMARK_TEMPLATE(LFSR_FeedthroughFibonacciBulk, Degree_46)MIN_TIME;
BENCHMARK_TEMPLATE(LFSR_FeedthroughFibonacciBulk, Degree_47)MIN_TIME;
BENCHMARK_TEMPLATE(LFSR_FeedthroughFibonacciBulk, Degree_48)MIN_TIME;
BENCHMARK_TEMPLATE(LFSR_FeedthroughFibonacciBulk, Degree_49)MIN_TIME;
BENCHMARK_TEMPLATE(LFSR_FeedthroughFibonacciBulk, Degree_50)MIN_TIME;
BENCHMARK_TEMPLATE(LFSR_FeedthroughFibonacciBulk, Degree_51)MIN_TIME;
BENCHMARK_TEMPLATE(LFSR_FeedthroughFibonacciBulk, Degree_52)MIN_TIME;
BENCHMARK_TEMPLATE(LFSR_FeedthroughFibonacciBulk, Degree_53)MIN_TIME;
BENCHMARK_TEMPLATE(LFSR_FeedthroughFibonacciBulk, Degree_54)MIN_TIME;
BENCHMARK_TEMPLATE(LFSR_FeedthroughFibonacciBulk, Degree_55)MIN_TIME;
BENCHMARK_TEMPLATE(LFSR_FeedthroughFibonacciBulk, Degree_56)MIN_TIME;
BENCHMARK_TEMPLATE(LFSR_FeedthroughFibonacciBulk, Degree_57)MIN_TIME;
BENCHMARK_TEMPLATE(LFSR_FeedthroughFibonacciBulk, Degree_58)MIN_TIME;
BENCHMARK_TEMPLATE(LFSR_FeedthroughFibonacciBulk, Degree_59)MIN_TIME;
BENCHMARK_TEMPLATE(LFSR_FeedthroughFibonacciBulk, Degree_60)MIN_TIME;
BENCHMARK_TEMPLATE(LFSR_FeedthroughFibonacciBulk, Degree_61)MIN_TIME;
BENCHMARK_TEMPLATE(LFSR_FeedthroughFibonacciBulk, Degree_62)MIN_TIME;
BENCHMARK_TEMPLATE(LFSR_FeedthroughFibonacciBulk, Degree_63)MIN_TIME;

BENCHMARK_MAIN();