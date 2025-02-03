#pragma once

#include <lfsr.hpp>


#include <cstddef>

namespace detail {

/* Primitive polynomials as listed on Wikipedia up to degree 24. */
using taps_0_1_2            = lfsr::tap_list<0, 1, 2>;
using taps_0_2_3            = lfsr::tap_list<0, 2, 3>;
using taps_0_3_4            = lfsr::tap_list<0, 3, 4>;
using taps_0_3_5            = lfsr::tap_list<0, 3, 5>;
using taps_0_5_6            = lfsr::tap_list<0, 5, 6>;
using taps_0_6_7            = lfsr::tap_list<0, 6, 7>;
using taps_0_4_5_6_8        = lfsr::tap_list<0, 4, 5, 6, 8>;
using taps_0_5_9            = lfsr::tap_list<0, 5, 9>;
using taps_0_7_10           = lfsr::tap_list<0, 7, 10>;
using taps_0_9_11           = lfsr::tap_list<0, 9, 11>;
using taps_0_4_10_11_12     = lfsr::tap_list<0, 4, 10, 11, 12>;
using taps_0_8_11_12_13     = lfsr::tap_list<0, 8, 11, 12, 13>;
using taps_0_2_12_13_14     = lfsr::tap_list<0, 2, 12, 13, 14>;
using taps_0_14_15          = lfsr::tap_list<0, 14, 15>;
using taps_0_4_13_15_16     = lfsr::tap_list<0, 4, 13, 15, 16>;
using taps_0_14_17          = lfsr::tap_list<0, 14, 17>;
using taps_0_11_18          = lfsr::tap_list<0, 11, 18>;
using taps_0_14_17_18_19    = lfsr::tap_list<0, 14, 17, 18, 19>;
using taps_0_17_20          = lfsr::tap_list<0, 17, 20>;
using taps_0_19_21          = lfsr::tap_list<0, 19, 21>;
using taps_0_21_22          = lfsr::tap_list<0, 21, 22>;
using taps_0_18_23          = lfsr::tap_list<0, 18, 23>;
using taps_0_17_20_22_23_24 = lfsr::tap_list<0, 17, 22, 23, 24>;









// https://stackoverflow.com/questions/72906388/why-cant-i-substitute-a-string-into-a-nttp
template<std::size_t n>
struct fixed_string {
    constexpr fixed_string() = default;
    constexpr fixed_string(const char(&str)[n + 1]) noexcept {
        auto i = std::size_t{0};
        for (char const c : str) {
            _data[i++] = c;
        }
        _data[n] = static_cast<char>(0);
    }
    
    friend constexpr auto operator<=>(fixed_string const&, fixed_string const&) = default;
    friend constexpr auto operator==(fixed_string const&, fixed_string const&) -> bool = default;
    
    [[nodiscard]]
    static constexpr auto size() noexcept -> std::size_t {
        return n;
    }

    [[nodiscard]]
    static constexpr auto empty() noexcept -> bool {
        return n == 0;
    }
    
    constexpr auto data() const& noexcept -> char const* {
        return _data;
    }
    
    constexpr auto data() & noexcept -> char* {
        return _data;
    }
    
    constexpr auto begin() const& noexcept -> char const* {
        return _data;
    }
    
    constexpr auto end() const& noexcept -> char const* {
        return _data + n;
    }
    
    constexpr auto begin() & noexcept -> char* {
        return _data;
    }
    
    constexpr auto end() & noexcept -> char* {
        return _data + n;
    }
    
    constexpr auto operator[](std::size_t index) noexcept {
        return _data[index];
    }

    auto to_string() const -> std::string {
        return std::string{_data};
    }
    
    char _data[n + 1];
};

// template argument deduction
template<std::size_t n>
fixed_string(char const(&)[n]) -> fixed_string<n - 1>;










template <typename TapList>
struct feedthrough_fibonacci_bulk_from_list;

template <std::size_t ... Taps>
struct feedthrough_fibonacci_bulk_from_list<lfsr::tap_list<Taps...>>
{
    using type = lfsr::feedthrough_fibonacci_bulk<Taps...>;
};

template <typename TapList>
using feedthrough_fibonacci_bulk_from_list_t 
    = typename feedthrough_fibonacci_bulk_from_list<TapList>::type;





template <typename TapList>
struct feedthrough_fibonacci_from_list;

template <std::size_t ... Taps>
struct feedthrough_fibonacci_from_list<lfsr::tap_list<Taps...>>
{
    using type = lfsr::feedthrough_fibonacci<Taps...>;
};

template <typename TapList>
using feedthrough_fibonacci_from_list_t 
    = typename feedthrough_fibonacci_from_list<TapList>::type;





template <typename TapList>
struct feedthrough_galois_from_list;

template <std::size_t ... Taps>
struct feedthrough_galois_from_list<lfsr::tap_list<Taps...>>
{
    using type = lfsr::feedthrough_galois<Taps...>;
};

template <typename TapList>
using feedthrough_galois_from_list_t 
    = typename feedthrough_galois_from_list<TapList>::type;




template <fixed_string BaseName>
struct NameGenerator 
{
    template <typename T>
    struct NameGeneratorImpl;

    template <std::size_t ... Taps>
    struct NameGeneratorImpl<lfsr::tap_list<Taps...>>
    {
        static auto GetName(int) -> std::string
        {
            auto taps = std::vector<std::size_t>{Taps...};
            auto base = BaseName.to_string();
            base += "<";
            for (auto it = taps.begin(); it != taps.end(); ++it) {
                if (it != taps.begin()) {
                    base += ", ";
                }
                base += std::format("{}", *it);
            }
            base += ">";
            return base;
        }
    };
    template <typename TapList>
    static auto GetName(int) -> std::string
    {
        return NameGeneratorImpl<TapList>::GetName(0);
    }
};


}