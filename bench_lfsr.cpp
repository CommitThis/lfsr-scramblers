#include <lfsr.hpp>

#include <test_detail.hpp>

#include <benchmark/benchmark.h>


using tap_list = lfsr::tap_list<0, 4, 10, 11, 12>;


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

auto LFSR_FeedthroughGalois(benchmark::State & state)
{
    using lfsr_type = detail::feedthrough_galois_from_list_t<tap_list>;

    auto lfsr = lfsr_type{};
    for (auto _ : state) {
        auto x = lfsr.scramble_byte(42);
        benchmark::DoNotOptimize(x);
    }
}

auto LFSR_FeedthroughFibonacci(benchmark::State & state)
{
    using lfsr_type = detail::feedthrough_fibonacci_from_list_t<tap_list>;

    auto lfsr = lfsr_type{};
    for (auto _ : state) {
        auto x = lfsr.scramble_byte(42);
        benchmark::DoNotOptimize(x);
    }
}

auto LFSR_FeedthroughFibonacciBulk(benchmark::State & state)
{
    using lfsr_type = detail::feedthrough_fibonacci_bulk_from_list_t<tap_list>;

    auto lfsr = lfsr_type{};
    for (auto _ : state) {
        auto x = lfsr.scramble_byte(42);
        benchmark::DoNotOptimize(x);
    }
}


auto LFSR_FeedthroughGalois_Range(benchmark::State & state)
{
    using lfsr_type = detail::feedthrough_galois_from_list_t<tap_list>;

    auto result = std::vector<std::uint8_t>();
    auto lfsr = lfsr_type{};
    for (auto _ : state) {
        lfsr.scramble_range(std::begin(test_data), 
                std::end(test_data), 
                std::back_inserter(result));
        benchmark::DoNotOptimize(result);
    }
}


auto LFSR_FeedthroughFibonacci_Range(benchmark::State & state)
{
    using lfsr_type = detail::feedthrough_fibonacci_from_list_t<tap_list>;

    auto result = std::vector<std::uint8_t>();
    auto lfsr = lfsr_type{};
    for (auto _ : state) {
        lfsr.scramble_range(std::begin(test_data), 
                std::end(test_data), 
                std::back_inserter(result));
        benchmark::DoNotOptimize(result);
    }
}


auto LFSR_FeedthroughFibonacciBulk_Range(benchmark::State & state)
{
    using lfsr_type = detail::feedthrough_fibonacci_bulk_from_list_t<tap_list>;

    auto result = std::vector<std::uint8_t>();
    auto lfsr = lfsr_type{};
    for (auto _ : state) {
        lfsr.scramble_range(std::begin(test_data), 
                std::end(test_data), 
                std::back_inserter(result));
        benchmark::DoNotOptimize(result);
    }
}



BENCHMARK(LFSR_FeedthroughGalois);
BENCHMARK(LFSR_FeedthroughFibonacci);
BENCHMARK(LFSR_FeedthroughFibonacciBulk);

BENCHMARK(LFSR_FeedthroughGalois_Range);
BENCHMARK(LFSR_FeedthroughFibonacci_Range);
BENCHMARK(LFSR_FeedthroughFibonacciBulk_Range);

BENCHMARK_MAIN();