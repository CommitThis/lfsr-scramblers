#include <lfsr.hpp>

#include <test_detail.hpp>

#include <filesystem>
#include <fstream>


namespace fs = std::filesystem;

using tap_list = lfsr::tap_list<0, 17, 20>;


auto run_galois(std::vector<std::uint8_t> const & to_scramble)
{
    using lfsr_type = detail::feedthrough_galois_from_list_t<tap_list>;

    auto lfsr = lfsr_type{};
    auto scrambled = std::vector<std::uint8_t>{};
    lfsr.scramble_range(std::begin(to_scramble), 
            std::end(to_scramble), 
            std::back_inserter(scrambled));

    auto out = std::fstream{"scrambled.galois.bin", std::ios::out | std::ios::binary};
    out.write(reinterpret_cast<char const *>(scrambled.data()), scrambled.size());
}


auto run_fibonacci(std::vector<std::uint8_t> const & to_scramble)
{
    using lfsr_type = detail::feedthrough_fibonacci_from_list_t<tap_list>;

    auto lfsr = lfsr_type{};
    auto scrambled = std::vector<std::uint8_t>{};
    lfsr.scramble_range(std::begin(to_scramble), 
            std::end(to_scramble), 
            std::back_inserter(scrambled));

    auto out = std::fstream{"scrambled.fibonacci.bin", std::ios::out | std::ios::binary};
    out.write(reinterpret_cast<char const *>(scrambled.data()), scrambled.size());
}


auto run_fibonacci_bulk(std::vector<std::uint8_t> const & to_scramble)
{
    using lfsr_type = detail::feedthrough_fibonacci_bulk_from_list_t<tap_list>;

    auto lfsr = lfsr_type{};
    auto scrambled = std::vector<std::uint8_t>{};
    lfsr.scramble_range(std::begin(to_scramble), 
            std::end(to_scramble), 
            std::back_inserter(scrambled));

    auto out = std::fstream{"scrambled.fibonaccibulk.bin", std::ios::out | std::ios::binary};
    out.write(reinterpret_cast<char const *>(scrambled.data()), scrambled.size());
}



auto main() -> int
{
    auto to_scramble = std::vector<std::uint8_t>{};
    for (auto ii = 0; ii != 1024 * 1024 * 100; ++ii) {
        to_scramble.push_back(ii);
    }

    run_fibonacci(to_scramble);
    run_galois(to_scramble);
    run_fibonacci_bulk(to_scramble);
}