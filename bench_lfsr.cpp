#include <lfsr.hpp>

#include <test_detail.hpp>
#include <bench_detail.hpp>

#include <benchmark/benchmark.h>



template <typename T, std::size_t N>
auto make_array(T const (&arr)[N]) -> std::array<T, N>
{
    auto result = std::array<T, N>{};
    for (auto ii = 0ul; ii != N; ++ii) {
        result[ii] = arr[ii];
    }
    return result;
}

const auto test_data = make_array<std::uint8_t>({
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
    120, 121, 122, 123, 124, 125, 126, 127,
});




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
    result.resize(test_data.size());
    auto lfsr = lfsr_type{};
    for (auto _ : state) {
        lfsr.scramble_range(std::begin(test_data), 
                std::end(test_data), 
                std::begin(result));
        benchmark::DoNotOptimize(result);
    }
    state.SetBytesProcessed(test_data.size() * state.iterations());

    if (result == all_zeroes()) {
        throw std::runtime_error("result == all_zeroes()");
    }
}


template <typename TapList>
auto LFSR_FeedthroughFibonacci(benchmark::State & state)
{
    using lfsr_type = detail::feedthrough_fibonacci_from_list_t<TapList>;

    auto result = std::vector<std::uint8_t>();
    result.resize(test_data.size());
    auto lfsr = lfsr_type{};
    for (auto _ : state) {
        lfsr.scramble_range(std::begin(test_data), 
                std::end(test_data), 
                std::begin(result));
        benchmark::DoNotOptimize(result);
    }
    state.SetBytesProcessed(test_data.size() * state.iterations());

    if (result == all_zeroes()) {
        throw std::runtime_error("result == all_zeroes()");
    }
}


template <typename TapList>
auto LFSR_FeedthroughFibonacciBulk(benchmark::State & state)
{
    using lfsr_type = detail::feedthrough_fibonacci_bulk_from_list_t<TapList>;

    auto result = std::vector<std::uint8_t>();
    result.resize(test_data.size());
    auto lfsr = lfsr_type{};
    for (auto _ : state) {
        lfsr.scramble_range(std::begin(test_data), 
                std::end(test_data), 
                std::begin(result));
        benchmark::DoNotOptimize(result);
    }
    state.SetBytesProcessed(test_data.size() * state.iterations());
    // state.

    if (result == all_zeroes()) {
        throw std::runtime_error("result == all_zeroes()");
    }
}




// #define TEST_OPTS ->MinTime(0.1)
// #define TEST_OPTS ->MinTime(2.0)

/* We need to use real time. On Windows/MSVC, the performance counter used 
   for CPU time is `GetThreadTime` which has an accuracy which is terrible
   for microbrnchmarks. Not only will the time per run be completely off,
   but so will any synthetic measurement that depends on it, notably,
   any kind of throughput calculation. */
#define TEST_OPTS ->UseRealTime()

BENCHMARK_TEMPLATE(LFSR_FeedthroughGalois, Degree_5)TEST_OPTS;
BENCHMARK_TEMPLATE(LFSR_FeedthroughGalois, Degree_6)TEST_OPTS;
BENCHMARK_TEMPLATE(LFSR_FeedthroughGalois, Degree_7)TEST_OPTS;
BENCHMARK_TEMPLATE(LFSR_FeedthroughGalois, Degree_8)TEST_OPTS;
BENCHMARK_TEMPLATE(LFSR_FeedthroughGalois, Degree_9)TEST_OPTS;
BENCHMARK_TEMPLATE(LFSR_FeedthroughGalois, Degree_10)TEST_OPTS;
BENCHMARK_TEMPLATE(LFSR_FeedthroughGalois, Degree_11)TEST_OPTS;
BENCHMARK_TEMPLATE(LFSR_FeedthroughGalois, Degree_12)TEST_OPTS;
BENCHMARK_TEMPLATE(LFSR_FeedthroughGalois, Degree_13)TEST_OPTS;
BENCHMARK_TEMPLATE(LFSR_FeedthroughGalois, Degree_14)TEST_OPTS;
BENCHMARK_TEMPLATE(LFSR_FeedthroughGalois, Degree_15)TEST_OPTS;
BENCHMARK_TEMPLATE(LFSR_FeedthroughGalois, Degree_16)TEST_OPTS;
BENCHMARK_TEMPLATE(LFSR_FeedthroughGalois, Degree_17)TEST_OPTS;
BENCHMARK_TEMPLATE(LFSR_FeedthroughGalois, Degree_18)TEST_OPTS;
BENCHMARK_TEMPLATE(LFSR_FeedthroughGalois, Degree_19)TEST_OPTS;
BENCHMARK_TEMPLATE(LFSR_FeedthroughGalois, Degree_20)TEST_OPTS;
BENCHMARK_TEMPLATE(LFSR_FeedthroughGalois, Degree_21)TEST_OPTS;
BENCHMARK_TEMPLATE(LFSR_FeedthroughGalois, Degree_22)TEST_OPTS;
BENCHMARK_TEMPLATE(LFSR_FeedthroughGalois, Degree_23)TEST_OPTS;
BENCHMARK_TEMPLATE(LFSR_FeedthroughGalois, Degree_24)TEST_OPTS;
BENCHMARK_TEMPLATE(LFSR_FeedthroughGalois, Degree_25)TEST_OPTS;
BENCHMARK_TEMPLATE(LFSR_FeedthroughGalois, Degree_26)TEST_OPTS;
BENCHMARK_TEMPLATE(LFSR_FeedthroughGalois, Degree_27)TEST_OPTS;
BENCHMARK_TEMPLATE(LFSR_FeedthroughGalois, Degree_28)TEST_OPTS;
BENCHMARK_TEMPLATE(LFSR_FeedthroughGalois, Degree_29)TEST_OPTS;
BENCHMARK_TEMPLATE(LFSR_FeedthroughGalois, Degree_30)TEST_OPTS;
BENCHMARK_TEMPLATE(LFSR_FeedthroughGalois, Degree_31)TEST_OPTS;
BENCHMARK_TEMPLATE(LFSR_FeedthroughGalois, Degree_32)TEST_OPTS;
BENCHMARK_TEMPLATE(LFSR_FeedthroughGalois, Degree_33)TEST_OPTS;
BENCHMARK_TEMPLATE(LFSR_FeedthroughGalois, Degree_34)TEST_OPTS;
BENCHMARK_TEMPLATE(LFSR_FeedthroughGalois, Degree_35)TEST_OPTS;
BENCHMARK_TEMPLATE(LFSR_FeedthroughGalois, Degree_36)TEST_OPTS;
BENCHMARK_TEMPLATE(LFSR_FeedthroughGalois, Degree_37)TEST_OPTS;
BENCHMARK_TEMPLATE(LFSR_FeedthroughGalois, Degree_38)TEST_OPTS;
BENCHMARK_TEMPLATE(LFSR_FeedthroughGalois, Degree_39)TEST_OPTS;
BENCHMARK_TEMPLATE(LFSR_FeedthroughGalois, Degree_40)TEST_OPTS;
BENCHMARK_TEMPLATE(LFSR_FeedthroughGalois, Degree_41)TEST_OPTS;
BENCHMARK_TEMPLATE(LFSR_FeedthroughGalois, Degree_42)TEST_OPTS;
BENCHMARK_TEMPLATE(LFSR_FeedthroughGalois, Degree_43)TEST_OPTS;
BENCHMARK_TEMPLATE(LFSR_FeedthroughGalois, Degree_44)TEST_OPTS;
BENCHMARK_TEMPLATE(LFSR_FeedthroughGalois, Degree_45)TEST_OPTS;
BENCHMARK_TEMPLATE(LFSR_FeedthroughGalois, Degree_46)TEST_OPTS;
BENCHMARK_TEMPLATE(LFSR_FeedthroughGalois, Degree_47)TEST_OPTS;
BENCHMARK_TEMPLATE(LFSR_FeedthroughGalois, Degree_48)TEST_OPTS;
BENCHMARK_TEMPLATE(LFSR_FeedthroughGalois, Degree_49)TEST_OPTS;
BENCHMARK_TEMPLATE(LFSR_FeedthroughGalois, Degree_50)TEST_OPTS;
BENCHMARK_TEMPLATE(LFSR_FeedthroughGalois, Degree_51)TEST_OPTS;
BENCHMARK_TEMPLATE(LFSR_FeedthroughGalois, Degree_52)TEST_OPTS;
BENCHMARK_TEMPLATE(LFSR_FeedthroughGalois, Degree_53)TEST_OPTS;
BENCHMARK_TEMPLATE(LFSR_FeedthroughGalois, Degree_54)TEST_OPTS;
BENCHMARK_TEMPLATE(LFSR_FeedthroughGalois, Degree_55)TEST_OPTS;
BENCHMARK_TEMPLATE(LFSR_FeedthroughGalois, Degree_56)TEST_OPTS;
BENCHMARK_TEMPLATE(LFSR_FeedthroughGalois, Degree_57)TEST_OPTS;
BENCHMARK_TEMPLATE(LFSR_FeedthroughGalois, Degree_58)TEST_OPTS;
BENCHMARK_TEMPLATE(LFSR_FeedthroughGalois, Degree_59)TEST_OPTS;
BENCHMARK_TEMPLATE(LFSR_FeedthroughGalois, Degree_60)TEST_OPTS;
BENCHMARK_TEMPLATE(LFSR_FeedthroughGalois, Degree_61)TEST_OPTS;
BENCHMARK_TEMPLATE(LFSR_FeedthroughGalois, Degree_62)TEST_OPTS;
BENCHMARK_TEMPLATE(LFSR_FeedthroughGalois, Degree_63)TEST_OPTS;


BENCHMARK_TEMPLATE(LFSR_FeedthroughFibonacci, Degree_5)TEST_OPTS;;
BENCHMARK_TEMPLATE(LFSR_FeedthroughFibonacci, Degree_6)TEST_OPTS;
BENCHMARK_TEMPLATE(LFSR_FeedthroughFibonacci, Degree_7)TEST_OPTS;
BENCHMARK_TEMPLATE(LFSR_FeedthroughFibonacci, Degree_8)TEST_OPTS;
BENCHMARK_TEMPLATE(LFSR_FeedthroughFibonacci, Degree_9)TEST_OPTS;
BENCHMARK_TEMPLATE(LFSR_FeedthroughFibonacci, Degree_10)TEST_OPTS;
BENCHMARK_TEMPLATE(LFSR_FeedthroughFibonacci, Degree_11)TEST_OPTS;
BENCHMARK_TEMPLATE(LFSR_FeedthroughFibonacci, Degree_12)TEST_OPTS;
BENCHMARK_TEMPLATE(LFSR_FeedthroughFibonacci, Degree_13)TEST_OPTS;
BENCHMARK_TEMPLATE(LFSR_FeedthroughFibonacci, Degree_14)TEST_OPTS;
BENCHMARK_TEMPLATE(LFSR_FeedthroughFibonacci, Degree_15)TEST_OPTS;
BENCHMARK_TEMPLATE(LFSR_FeedthroughFibonacci, Degree_16)TEST_OPTS;
BENCHMARK_TEMPLATE(LFSR_FeedthroughFibonacci, Degree_17)TEST_OPTS;
BENCHMARK_TEMPLATE(LFSR_FeedthroughFibonacci, Degree_18)TEST_OPTS;
BENCHMARK_TEMPLATE(LFSR_FeedthroughFibonacci, Degree_19)TEST_OPTS;
BENCHMARK_TEMPLATE(LFSR_FeedthroughFibonacci, Degree_20)TEST_OPTS;
BENCHMARK_TEMPLATE(LFSR_FeedthroughFibonacci, Degree_21)TEST_OPTS;
BENCHMARK_TEMPLATE(LFSR_FeedthroughFibonacci, Degree_22)TEST_OPTS;
BENCHMARK_TEMPLATE(LFSR_FeedthroughFibonacci, Degree_23)TEST_OPTS;
BENCHMARK_TEMPLATE(LFSR_FeedthroughFibonacci, Degree_24)TEST_OPTS;
BENCHMARK_TEMPLATE(LFSR_FeedthroughFibonacci, Degree_25)TEST_OPTS;
BENCHMARK_TEMPLATE(LFSR_FeedthroughFibonacci, Degree_26)TEST_OPTS;
BENCHMARK_TEMPLATE(LFSR_FeedthroughFibonacci, Degree_27)TEST_OPTS;
BENCHMARK_TEMPLATE(LFSR_FeedthroughFibonacci, Degree_28)TEST_OPTS;
BENCHMARK_TEMPLATE(LFSR_FeedthroughFibonacci, Degree_29)TEST_OPTS;
BENCHMARK_TEMPLATE(LFSR_FeedthroughFibonacci, Degree_30)TEST_OPTS;
BENCHMARK_TEMPLATE(LFSR_FeedthroughFibonacci, Degree_31)TEST_OPTS;
BENCHMARK_TEMPLATE(LFSR_FeedthroughFibonacci, Degree_32)TEST_OPTS;
BENCHMARK_TEMPLATE(LFSR_FeedthroughFibonacci, Degree_33)TEST_OPTS;
BENCHMARK_TEMPLATE(LFSR_FeedthroughFibonacci, Degree_34)TEST_OPTS;
BENCHMARK_TEMPLATE(LFSR_FeedthroughFibonacci, Degree_35)TEST_OPTS;
BENCHMARK_TEMPLATE(LFSR_FeedthroughFibonacci, Degree_36)TEST_OPTS;
BENCHMARK_TEMPLATE(LFSR_FeedthroughFibonacci, Degree_37)TEST_OPTS;
BENCHMARK_TEMPLATE(LFSR_FeedthroughFibonacci, Degree_38)TEST_OPTS;
BENCHMARK_TEMPLATE(LFSR_FeedthroughFibonacci, Degree_39)TEST_OPTS;
BENCHMARK_TEMPLATE(LFSR_FeedthroughFibonacci, Degree_40)TEST_OPTS;
BENCHMARK_TEMPLATE(LFSR_FeedthroughFibonacci, Degree_41)TEST_OPTS;
BENCHMARK_TEMPLATE(LFSR_FeedthroughFibonacci, Degree_42)TEST_OPTS;
BENCHMARK_TEMPLATE(LFSR_FeedthroughFibonacci, Degree_43)TEST_OPTS;
BENCHMARK_TEMPLATE(LFSR_FeedthroughFibonacci, Degree_44)TEST_OPTS;
BENCHMARK_TEMPLATE(LFSR_FeedthroughFibonacci, Degree_45)TEST_OPTS;
BENCHMARK_TEMPLATE(LFSR_FeedthroughFibonacci, Degree_46)TEST_OPTS;
BENCHMARK_TEMPLATE(LFSR_FeedthroughFibonacci, Degree_47)TEST_OPTS;
BENCHMARK_TEMPLATE(LFSR_FeedthroughFibonacci, Degree_48)TEST_OPTS;
BENCHMARK_TEMPLATE(LFSR_FeedthroughFibonacci, Degree_49)TEST_OPTS;
BENCHMARK_TEMPLATE(LFSR_FeedthroughFibonacci, Degree_50)TEST_OPTS;
BENCHMARK_TEMPLATE(LFSR_FeedthroughFibonacci, Degree_51)TEST_OPTS;
BENCHMARK_TEMPLATE(LFSR_FeedthroughFibonacci, Degree_52)TEST_OPTS;
BENCHMARK_TEMPLATE(LFSR_FeedthroughFibonacci, Degree_53)TEST_OPTS;
BENCHMARK_TEMPLATE(LFSR_FeedthroughFibonacci, Degree_54)TEST_OPTS;
BENCHMARK_TEMPLATE(LFSR_FeedthroughFibonacci, Degree_55)TEST_OPTS;
BENCHMARK_TEMPLATE(LFSR_FeedthroughFibonacci, Degree_56)TEST_OPTS;
BENCHMARK_TEMPLATE(LFSR_FeedthroughFibonacci, Degree_57)TEST_OPTS;
BENCHMARK_TEMPLATE(LFSR_FeedthroughFibonacci, Degree_58)TEST_OPTS;
BENCHMARK_TEMPLATE(LFSR_FeedthroughFibonacci, Degree_59)TEST_OPTS;
BENCHMARK_TEMPLATE(LFSR_FeedthroughFibonacci, Degree_60)TEST_OPTS;
BENCHMARK_TEMPLATE(LFSR_FeedthroughFibonacci, Degree_61)TEST_OPTS;
BENCHMARK_TEMPLATE(LFSR_FeedthroughFibonacci, Degree_62)TEST_OPTS;
BENCHMARK_TEMPLATE(LFSR_FeedthroughFibonacci, Degree_63)TEST_OPTS;

BENCHMARK_TEMPLATE(LFSR_FeedthroughFibonacciBulk, Degree_5)TEST_OPTS;;
BENCHMARK_TEMPLATE(LFSR_FeedthroughFibonacciBulk, Degree_6)TEST_OPTS;
BENCHMARK_TEMPLATE(LFSR_FeedthroughFibonacciBulk, Degree_7)TEST_OPTS;
BENCHMARK_TEMPLATE(LFSR_FeedthroughFibonacciBulk, Degree_8)TEST_OPTS;
BENCHMARK_TEMPLATE(LFSR_FeedthroughFibonacciBulk, Degree_9)TEST_OPTS;
BENCHMARK_TEMPLATE(LFSR_FeedthroughFibonacciBulk, Degree_10)TEST_OPTS;
BENCHMARK_TEMPLATE(LFSR_FeedthroughFibonacciBulk, Degree_11)TEST_OPTS;
BENCHMARK_TEMPLATE(LFSR_FeedthroughFibonacciBulk, Degree_12)TEST_OPTS;
BENCHMARK_TEMPLATE(LFSR_FeedthroughFibonacciBulk, Degree_13)TEST_OPTS;
BENCHMARK_TEMPLATE(LFSR_FeedthroughFibonacciBulk, Degree_14)TEST_OPTS;
BENCHMARK_TEMPLATE(LFSR_FeedthroughFibonacciBulk, Degree_15)TEST_OPTS;
BENCHMARK_TEMPLATE(LFSR_FeedthroughFibonacciBulk, Degree_16)TEST_OPTS;
BENCHMARK_TEMPLATE(LFSR_FeedthroughFibonacciBulk, Degree_17)TEST_OPTS;
BENCHMARK_TEMPLATE(LFSR_FeedthroughFibonacciBulk, Degree_18)TEST_OPTS;
BENCHMARK_TEMPLATE(LFSR_FeedthroughFibonacciBulk, Degree_19)TEST_OPTS;
BENCHMARK_TEMPLATE(LFSR_FeedthroughFibonacciBulk, Degree_20)TEST_OPTS;
BENCHMARK_TEMPLATE(LFSR_FeedthroughFibonacciBulk, Degree_21)TEST_OPTS;
BENCHMARK_TEMPLATE(LFSR_FeedthroughFibonacciBulk, Degree_22)TEST_OPTS;
BENCHMARK_TEMPLATE(LFSR_FeedthroughFibonacciBulk, Degree_23)TEST_OPTS;
BENCHMARK_TEMPLATE(LFSR_FeedthroughFibonacciBulk, Degree_24)TEST_OPTS;
BENCHMARK_TEMPLATE(LFSR_FeedthroughFibonacciBulk, Degree_25)TEST_OPTS;
BENCHMARK_TEMPLATE(LFSR_FeedthroughFibonacciBulk, Degree_26)TEST_OPTS;
BENCHMARK_TEMPLATE(LFSR_FeedthroughFibonacciBulk, Degree_27)TEST_OPTS;
BENCHMARK_TEMPLATE(LFSR_FeedthroughFibonacciBulk, Degree_28)TEST_OPTS;
BENCHMARK_TEMPLATE(LFSR_FeedthroughFibonacciBulk, Degree_29)TEST_OPTS;
BENCHMARK_TEMPLATE(LFSR_FeedthroughFibonacciBulk, Degree_30)TEST_OPTS;
BENCHMARK_TEMPLATE(LFSR_FeedthroughFibonacciBulk, Degree_31)TEST_OPTS;
BENCHMARK_TEMPLATE(LFSR_FeedthroughFibonacciBulk, Degree_32)TEST_OPTS;
BENCHMARK_TEMPLATE(LFSR_FeedthroughFibonacciBulk, Degree_33)TEST_OPTS;
BENCHMARK_TEMPLATE(LFSR_FeedthroughFibonacciBulk, Degree_34)TEST_OPTS;
BENCHMARK_TEMPLATE(LFSR_FeedthroughFibonacciBulk, Degree_35)TEST_OPTS;
BENCHMARK_TEMPLATE(LFSR_FeedthroughFibonacciBulk, Degree_36)TEST_OPTS;
BENCHMARK_TEMPLATE(LFSR_FeedthroughFibonacciBulk, Degree_37)TEST_OPTS;
BENCHMARK_TEMPLATE(LFSR_FeedthroughFibonacciBulk, Degree_38)TEST_OPTS;
BENCHMARK_TEMPLATE(LFSR_FeedthroughFibonacciBulk, Degree_39)TEST_OPTS;
BENCHMARK_TEMPLATE(LFSR_FeedthroughFibonacciBulk, Degree_40)TEST_OPTS;
BENCHMARK_TEMPLATE(LFSR_FeedthroughFibonacciBulk, Degree_41)TEST_OPTS;
BENCHMARK_TEMPLATE(LFSR_FeedthroughFibonacciBulk, Degree_42)TEST_OPTS;
BENCHMARK_TEMPLATE(LFSR_FeedthroughFibonacciBulk, Degree_43)TEST_OPTS;
BENCHMARK_TEMPLATE(LFSR_FeedthroughFibonacciBulk, Degree_44)TEST_OPTS;
BENCHMARK_TEMPLATE(LFSR_FeedthroughFibonacciBulk, Degree_45)TEST_OPTS;
BENCHMARK_TEMPLATE(LFSR_FeedthroughFibonacciBulk, Degree_46)TEST_OPTS;
BENCHMARK_TEMPLATE(LFSR_FeedthroughFibonacciBulk, Degree_47)TEST_OPTS;
BENCHMARK_TEMPLATE(LFSR_FeedthroughFibonacciBulk, Degree_48)TEST_OPTS;
BENCHMARK_TEMPLATE(LFSR_FeedthroughFibonacciBulk, Degree_49)TEST_OPTS;
BENCHMARK_TEMPLATE(LFSR_FeedthroughFibonacciBulk, Degree_50)TEST_OPTS;
BENCHMARK_TEMPLATE(LFSR_FeedthroughFibonacciBulk, Degree_51)TEST_OPTS;
BENCHMARK_TEMPLATE(LFSR_FeedthroughFibonacciBulk, Degree_52)TEST_OPTS;
BENCHMARK_TEMPLATE(LFSR_FeedthroughFibonacciBulk, Degree_53)TEST_OPTS;
BENCHMARK_TEMPLATE(LFSR_FeedthroughFibonacciBulk, Degree_54)TEST_OPTS;
BENCHMARK_TEMPLATE(LFSR_FeedthroughFibonacciBulk, Degree_55)TEST_OPTS;
BENCHMARK_TEMPLATE(LFSR_FeedthroughFibonacciBulk, Degree_56)TEST_OPTS;
BENCHMARK_TEMPLATE(LFSR_FeedthroughFibonacciBulk, Degree_57)TEST_OPTS;
BENCHMARK_TEMPLATE(LFSR_FeedthroughFibonacciBulk, Degree_58)TEST_OPTS;
BENCHMARK_TEMPLATE(LFSR_FeedthroughFibonacciBulk, Degree_59)TEST_OPTS;
BENCHMARK_TEMPLATE(LFSR_FeedthroughFibonacciBulk, Degree_60)TEST_OPTS;
BENCHMARK_TEMPLATE(LFSR_FeedthroughFibonacciBulk, Degree_61)TEST_OPTS;
BENCHMARK_TEMPLATE(LFSR_FeedthroughFibonacciBulk, Degree_62)TEST_OPTS;
BENCHMARK_TEMPLATE(LFSR_FeedthroughFibonacciBulk, Degree_63)TEST_OPTS;


// BENCHMARK_TEMPLATE(LFSR_FeedthroughFibonacciBulkSHL, Degree_5)TEST_OPTS;;
// BENCHMARK_TEMPLATE(LFSR_FeedthroughFibonacciBulkSHL, Degree_6)TEST_OPTS;
// BENCHMARK_TEMPLATE(LFSR_FeedthroughFibonacciBulkSHL, Degree_7)TEST_OPTS;
// BENCHMARK_TEMPLATE(LFSR_FeedthroughFibonacciBulkSHL, Degree_8)TEST_OPTS;
// BENCHMARK_TEMPLATE(LFSR_FeedthroughFibonacciBulkSHL, Degree_9)TEST_OPTS;
// BENCHMARK_TEMPLATE(LFSR_FeedthroughFibonacciBulkSHL, Degree_10)TEST_OPTS;
// BENCHMARK_TEMPLATE(LFSR_FeedthroughFibonacciBulkSHL, Degree_11)TEST_OPTS;
// BENCHMARK_TEMPLATE(LFSR_FeedthroughFibonacciBulkSHL, Degree_12)TEST_OPTS;
// BENCHMARK_TEMPLATE(LFSR_FeedthroughFibonacciBulkSHL, Degree_13)TEST_OPTS;
// BENCHMARK_TEMPLATE(LFSR_FeedthroughFibonacciBulkSHL, Degree_14)TEST_OPTS;
// BENCHMARK_TEMPLATE(LFSR_FeedthroughFibonacciBulkSHL, Degree_15)TEST_OPTS;
// BENCHMARK_TEMPLATE(LFSR_FeedthroughFibonacciBulkSHL, Degree_16)TEST_OPTS;
// BENCHMARK_TEMPLATE(LFSR_FeedthroughFibonacciBulkSHL, Degree_17)TEST_OPTS;
// BENCHMARK_TEMPLATE(LFSR_FeedthroughFibonacciBulkSHL, Degree_18)TEST_OPTS;
// BENCHMARK_TEMPLATE(LFSR_FeedthroughFibonacciBulkSHL, Degree_19)TEST_OPTS;
// BENCHMARK_TEMPLATE(LFSR_FeedthroughFibonacciBulkSHL, Degree_20)TEST_OPTS;
// BENCHMARK_TEMPLATE(LFSR_FeedthroughFibonacciBulkSHL, Degree_21)TEST_OPTS;
// BENCHMARK_TEMPLATE(LFSR_FeedthroughFibonacciBulkSHL, Degree_22)TEST_OPTS;
// BENCHMARK_TEMPLATE(LFSR_FeedthroughFibonacciBulkSHL, Degree_23)TEST_OPTS;
// BENCHMARK_TEMPLATE(LFSR_FeedthroughFibonacciBulkSHL, Degree_24)TEST_OPTS;
// BENCHMARK_TEMPLATE(LFSR_FeedthroughFibonacciBulkSHL, Degree_25)TEST_OPTS;
// BENCHMARK_TEMPLATE(LFSR_FeedthroughFibonacciBulkSHL, Degree_26)TEST_OPTS;
// BENCHMARK_TEMPLATE(LFSR_FeedthroughFibonacciBulkSHL, Degree_27)TEST_OPTS;
// BENCHMARK_TEMPLATE(LFSR_FeedthroughFibonacciBulkSHL, Degree_28)TEST_OPTS;
// BENCHMARK_TEMPLATE(LFSR_FeedthroughFibonacciBulkSHL, Degree_29)TEST_OPTS;
// BENCHMARK_TEMPLATE(LFSR_FeedthroughFibonacciBulkSHL, Degree_30)TEST_OPTS;
// BENCHMARK_TEMPLATE(LFSR_FeedthroughFibonacciBulkSHL, Degree_31)TEST_OPTS;
// BENCHMARK_TEMPLATE(LFSR_FeedthroughFibonacciBulkSHL, Degree_32)TEST_OPTS;
// BENCHMARK_TEMPLATE(LFSR_FeedthroughFibonacciBulkSHL, Degree_33)TEST_OPTS;
// BENCHMARK_TEMPLATE(LFSR_FeedthroughFibonacciBulkSHL, Degree_34)TEST_OPTS;
// BENCHMARK_TEMPLATE(LFSR_FeedthroughFibonacciBulkSHL, Degree_35)TEST_OPTS;
// BENCHMARK_TEMPLATE(LFSR_FeedthroughFibonacciBulkSHL, Degree_36)TEST_OPTS;
// BENCHMARK_TEMPLATE(LFSR_FeedthroughFibonacciBulkSHL, Degree_37)TEST_OPTS;
// BENCHMARK_TEMPLATE(LFSR_FeedthroughFibonacciBulkSHL, Degree_38)TEST_OPTS;
// BENCHMARK_TEMPLATE(LFSR_FeedthroughFibonacciBulkSHL, Degree_39)TEST_OPTS;
// BENCHMARK_TEMPLATE(LFSR_FeedthroughFibonacciBulkSHL, Degree_40)TEST_OPTS;
// BENCHMARK_TEMPLATE(LFSR_FeedthroughFibonacciBulkSHL, Degree_41)TEST_OPTS;
// BENCHMARK_TEMPLATE(LFSR_FeedthroughFibonacciBulkSHL, Degree_42)TEST_OPTS;
// BENCHMARK_TEMPLATE(LFSR_FeedthroughFibonacciBulkSHL, Degree_43)TEST_OPTS;
// BENCHMARK_TEMPLATE(LFSR_FeedthroughFibonacciBulkSHL, Degree_44)TEST_OPTS;
// BENCHMARK_TEMPLATE(LFSR_FeedthroughFibonacciBulkSHL, Degree_45)TEST_OPTS;
// BENCHMARK_TEMPLATE(LFSR_FeedthroughFibonacciBulkSHL, Degree_46)TEST_OPTS;
// BENCHMARK_TEMPLATE(LFSR_FeedthroughFibonacciBulkSHL, Degree_47)TEST_OPTS;
// BENCHMARK_TEMPLATE(LFSR_FeedthroughFibonacciBulkSHL, Degree_48)TEST_OPTS;
// BENCHMARK_TEMPLATE(LFSR_FeedthroughFibonacciBulkSHL, Degree_49)TEST_OPTS;
// BENCHMARK_TEMPLATE(LFSR_FeedthroughFibonacciBulkSHL, Degree_50)TEST_OPTS;
// BENCHMARK_TEMPLATE(LFSR_FeedthroughFibonacciBulkSHL, Degree_51)TEST_OPTS;
// BENCHMARK_TEMPLATE(LFSR_FeedthroughFibonacciBulkSHL, Degree_52)TEST_OPTS;
// BENCHMARK_TEMPLATE(LFSR_FeedthroughFibonacciBulkSHL, Degree_53)TEST_OPTS;
// BENCHMARK_TEMPLATE(LFSR_FeedthroughFibonacciBulkSHL, Degree_54)TEST_OPTS;
// BENCHMARK_TEMPLATE(LFSR_FeedthroughFibonacciBulkSHL, Degree_55)TEST_OPTS;
// BENCHMARK_TEMPLATE(LFSR_FeedthroughFibonacciBulkSHL, Degree_56)TEST_OPTS;
// BENCHMARK_TEMPLATE(LFSR_FeedthroughFibonacciBulkSHL, Degree_57)TEST_OPTS;
// BENCHMARK_TEMPLATE(LFSR_FeedthroughFibonacciBulkSHL, Degree_58)TEST_OPTS;
// BENCHMARK_TEMPLATE(LFSR_FeedthroughFibonacciBulkSHL, Degree_59)TEST_OPTS;
// BENCHMARK_TEMPLATE(LFSR_FeedthroughFibonacciBulkSHL, Degree_60)TEST_OPTS;
// BENCHMARK_TEMPLATE(LFSR_FeedthroughFibonacciBulkSHL, Degree_61)TEST_OPTS;
// BENCHMARK_TEMPLATE(LFSR_FeedthroughFibonacciBulkSHL, Degree_62)TEST_OPTS;
// BENCHMARK_TEMPLATE(LFSR_FeedthroughFibonacciBulkSHL, Degree_63)TEST_OPTS;

BENCHMARK_MAIN();