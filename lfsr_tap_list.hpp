#pragma once

#include <algorithm>
#include <array>
#include <cstddef>


namespace lfsr {

template <std::size_t ... Taps>
struct tap_list
{
    template <std::size_t New>
    using add = tap_list<Taps..., New>;

    using to_indices = tap_list<(Taps - 1) ...>;

    constexpr static auto lowest() {
        return std::min({Taps...});
    }
    constexpr static auto highest() {
        return std::max({Taps...});
    }

    constexpr static auto values = 
            std::array<std::size_t, sizeof...(Taps)>{Taps...};
};


}