
#pragma once

#include <algorithm>
#include <array>
#include <bitset>
#include <cstddef>
#include <cstdint>
#include <map>
#include <set>
#include <vector>

#include <format>   // DEBUG
#include <iostream> // DEBUG

#include <lfsr_detail.hpp>

namespace lfsr {



template <std::size_t ... Taps>
struct lfsr_traits
{
    using tap_list = detail::make_tap_list_t<Taps...>;

    constexpr static auto degree      = std::max({Taps...});
    constexpr static auto char_bits   = sizeof(char) * 8;
    constexpr static auto buffer_size = degree;
    constexpr static auto tap_indices = tap_list::to_indices::values;
    constexpr static auto lowest_tap  = tap_list::lowest();

    using buffer_type = std::bitset<buffer_size>;

    /* For intialising buffer state to 0 */
    static auto all_ones() -> buffer_type
    {
        return buffer_type{}.set();
    }
};



template <std::size_t ... Taps>
struct lfsr_bulk_traits
{
    constexpr static auto degree = std::max({Taps...});
    constexpr static auto char_bits = sizeof(char) * 8;
    constexpr static auto buffer_size = degree + char_bits;

    using tap_list        = detail::make_tap_list_t<Taps...>;
    using buffer_type     = std::bitset<buffer_size>;
    using state_type      = std::bitset<degree>;
    using dependency_list = std::array<buffer_type, char_bits>;
    
    /* For intialising buffer state to 0 */
    static auto all_ones() -> buffer_type
    {
        return buffer_type{}.set();
    }

    static auto state_mask() -> buffer_type
    {
        auto result = buffer_type{};
        for (auto ii = 0; ii != degree; ++ii) {
            result.set(ii);
        }
        return result;
    }

    static auto scramble_dependencies() -> dependency_list
    {
        auto result = dependency_list{};

        /* For each input bit, calculate the dependencies in the register. This
           is intentionally done from the least significant bit to the most
           significant bit, as a more significant bit may end up having a 
           dependency on another input bit, so they need to be calculated first.
         */
        for (auto input_bit_index = 0ull; 
                  input_bit_index != char_bits; 
                  ++input_bit_index) 
        {
            auto tap_mask = buffer_type{};

            /* Index of the current input bit in the register */
            auto input_reg_index = input_bit_index + degree;

            for (auto tap : {Taps...}) 
            {
                /* Index of the current tap in the register, for the current
                   input bit index. */
                auto tap_reg_index = degree - tap + input_bit_index;

                /* If we encounter a tap dependency in the register that is also
                   a previous input, we can accumulate the dependencies by 
                   XORing that bits dependencies with the current mask. By
                   definition, this would be any tap that has a register index 
                   higher than the degree (or size of an inputless register). */
                if (tap_reg_index >= degree) 
                {
                    /* Index of the tap in the input byte */
                    auto tap_bit_index = tap_reg_index - degree;

                    /* Don't need to flip the current tap in the mask as the
                       dependency set already contains this. */
                    tap_mask ^= result[tap_bit_index];
                } else {
                    /* Otherwise, just flip the bit in the mask. */
                    tap_mask.flip(tap_reg_index);
                }
            }

            /* This is the actual input bit. */
            tap_mask.flip(input_reg_index);

            result[input_bit_index] = tap_mask;
        }

        return result;
    }


    static auto descramble_dependencies() -> dependency_list
    {
        auto result = dependency_list{};

        for (auto b = 0ull; b != char_bits; ++b) 
        {
            auto tap_mask  = buffer_type{};
            for (auto tap : {Taps...}) {
                tap_mask.flip(degree - tap + b);
            }

            result[b] = tap_mask;
        }

        return result;
    }
};





template <std::size_t ... Taps>
class feedthrough_fibonacci 
{
public:
    using traits      = lfsr_traits<Taps...>;
    using buffer_type = typename traits::buffer_type;

    constexpr static auto degree = traits::degree;
    constexpr static auto period = (1 << degree) - 1;

    feedthrough_fibonacci()
        : m_buffer{traits::all_ones()}
    {
    }

    auto state() -> std::bitset<degree>
    {
        return m_buffer;
    }

    auto scramble_bit(bool input) noexcept -> bool
    {
        /* Fibonacci implementation 
           This is the most straightforward way to implement an LFSR. The taps
           are processed in order, and the buffer is shifted to the left.

           Input bit is position 0, output bit is the position of last tap.
           Input bit is XORed with the bits at indicated tap positions.

           Can't use the counterpart taps as the input bit will be xor'd twice.
        */
        for (auto t : traits::tap_indices) {
            input ^= this->m_buffer.test(t); // not constexpr until C++23
        }
        this->m_buffer <<= 1;
        this->m_buffer.set(0, input);
        return input;
    }

    auto descramble_bit(bool input) noexcept -> bool
    {
        auto result = input;
        for (auto t : traits::tap_indices) {
            result ^= this->m_buffer.test(t); // not constexpr until C++23
        }

        this->m_buffer <<= 1;
        this->m_buffer.set(0, input);
        return result;
    };

    auto scramble_byte(std::uint8_t value) noexcept -> std::uint8_t
    {
        auto result = std::uint8_t{};
        for (auto ii = 0ull; ii != 8; ++ii) {
            result |= scramble_bit(value & 1) << ii;
            value >>= 1;
        }
        return result;
    }

    auto descramble_byte(std::uint8_t value) noexcept -> std::uint8_t
    {
        auto result = std::uint8_t{};
        for (auto ii = 0ull; ii != 8; ++ii) {
            result |= descramble_bit(value & 1) << ii;
            value >>= 1;
        }
        return result;
    }


    template <typename Iter1, typename Iter2>
        requires detail::iter_is_byte<Iter1>
    auto scramble_range(Iter1 first, Iter1 last, Iter2 d_first) noexcept -> void
    {
        for (; first != last; ++first, ++d_first) {
            *d_first = scramble_byte(*first);
        }
    }


    template <typename Iter1, typename Iter2>
        requires detail::iter_is_byte<Iter1>
    auto descramble_range(Iter1 first, Iter1 last, Iter2 d_first) noexcept -> void
    {
        for (; first != last; ++first, ++d_first) {
            *d_first = descramble_byte(*first);
        }
    }

    buffer_type m_buffer;
};





template <std::size_t ... Taps>
class feedthrough_galois 
{
public:
    using traits      = lfsr_traits<Taps...>;
    using buffer_type = typename traits::buffer_type;

    constexpr static auto degree = traits::degree;
    constexpr static auto period = (1 << degree) - 1;

    feedthrough_galois()
        : m_buffer{traits::all_ones()}
        , m_taps{}
    {
        for (auto t : traits::tap_indices) {
            this->m_taps.set(t);
        }
    }

    auto state() -> std::bitset<degree>
    {
        return m_buffer;
    }

    auto scramble_bit(bool input) noexcept -> bool
    {
        /* Galois implementation
           In order to get the same sequence as the Fibanacci implementation, we
           need to process the taps in reverse order. Because we want to keep
           the traits simple, we have to reverse the order in which they are
           processed which involves shifting the LFSR buffer to the right.

           Input bit is the position at the last tap, output bit is position 0.
        */
        auto out = this->m_buffer.test(0) ^ input;
        this->m_buffer >>= 1;
        if (out) {
            this->m_buffer ^= this->m_taps;
        }
        this->m_buffer.set(traits::degree - 1, out);
        return out;
    }

    auto descramble_bit(bool input) noexcept -> bool
    {
        /* Galois implementation */
        auto out = this->m_buffer.test(0) ^ input;
        this->m_buffer >>= 1;
        if (input) {
            this->m_buffer ^= this->m_taps;
        }
        this->m_buffer.set(traits::degree - 1, input);
        return out;
    };

    auto scramble_byte(std::uint8_t value) noexcept -> std::uint8_t
    {
        auto result = std::uint8_t{};
        for (auto ii = 0ull; ii != 8; ++ii) {
            result |= scramble_bit(value & 1) << ii;
            value >>= 1;
        }
        return result;
    }

    auto descramble_byte(std::uint8_t value) noexcept -> std::uint8_t
    {
        auto result = std::uint8_t{};
        for (auto ii = 0ull; ii != 8; ++ii) {
            result |= descramble_bit(value & 1) << ii;
            value >>= 1;
        }
        return result;
    }


    template <typename Iter1, typename Iter2>
        requires detail::iter_is_byte<Iter1>
    auto scramble_range(Iter1 first, Iter1 last, Iter2 d_first) noexcept -> void
    {
        for (; first != last; ++first, ++d_first) {
            *d_first = scramble_byte(*first);
        }
    }


    template <typename Iter1, typename Iter2>
        requires detail::iter_is_byte<Iter1>
    auto descramble_range(Iter1 first, Iter1 last, Iter2 d_first) noexcept -> void
    {
        for (; first != last; ++first, ++d_first) {
            *d_first = descramble_byte(*first);
        }
    }

    buffer_type m_buffer;
    buffer_type m_taps;
};












template <std::size_t ... Taps>
class feedthrough_fibonacci_bulk
{
public:
    using traits          = lfsr_bulk_traits<Taps...>;
    using buffer_type     = typename traits::buffer_type;
    using dependency_list = typename traits::dependency_list;

    constexpr static auto degree = traits::degree;
    constexpr static auto period = (1 << degree) - 1;

    feedthrough_fibonacci_bulk()
        : m_buffer{traits::all_ones()}
        , m_deps_scramble{traits::scramble_dependencies()}
        , m_deps_descramble{traits::descramble_dependencies()}
    {
    }

    auto state() -> std::bitset<degree>
    {
        static_assert(degree <= 64, "Cant use state() with degree > 64, "
                                    "state would overflow communicating type");
        return (m_buffer & traits::state_mask()).to_ullong();
    }

    auto scramble_bit(bool value) -> bool
    {
        m_buffer.set(degree, value);
        bool result = (m_buffer & m_deps_scramble[0]).count() & 1;
        m_buffer.set(degree, result);
        m_buffer >>= 1;
        return result;
    } 

    auto scramble_byte(std::uint8_t value) -> std::uint8_t
    {
        const static auto setmask = traits::state_mask();
        m_buffer >>= 8;
        m_buffer |= (buffer_type{value} << degree);

        auto result = std::uint8_t{};
        result |= ((m_buffer & m_deps_scramble[0]).count() & 1) << 0;
        result |= ((m_buffer & m_deps_scramble[1]).count() & 1) << 1;
        result |= ((m_buffer & m_deps_scramble[2]).count() & 1) << 2;
        result |= ((m_buffer & m_deps_scramble[3]).count() & 1) << 3;
        result |= ((m_buffer & m_deps_scramble[4]).count() & 1) << 4;
        result |= ((m_buffer & m_deps_scramble[5]).count() & 1) << 5;
        result |= ((m_buffer & m_deps_scramble[6]).count() & 1) << 6;
        result |= ((m_buffer & m_deps_scramble[7]).count() & 1) << 7;

        m_buffer = (m_buffer & setmask) | (buffer_type{result} << degree);
        return result;
    }



    auto descramble_byte(std::uint8_t value) -> std::uint8_t
    {
        m_buffer >>= 8;
        m_buffer |= (buffer_type{value} << degree);

        auto result = std::uint8_t{};
        result |= ((m_buffer & m_deps_descramble[0]).count() & 1) << 0;
        result |= ((m_buffer & m_deps_descramble[1]).count() & 1) << 1;
        result |= ((m_buffer & m_deps_descramble[2]).count() & 1) << 2;
        result |= ((m_buffer & m_deps_descramble[3]).count() & 1) << 3;
        result |= ((m_buffer & m_deps_descramble[4]).count() & 1) << 4;
        result |= ((m_buffer & m_deps_descramble[5]).count() & 1) << 5;
        result |= ((m_buffer & m_deps_descramble[6]).count() & 1) << 6;
        result |= ((m_buffer & m_deps_descramble[7]).count() & 1) << 7;

        return result;
    }

    auto descramble_bit(bool value) -> bool
    {
        m_buffer.set(degree, value);
        bool result = (m_buffer & m_deps_descramble[0]).count() & 1;
        m_buffer.set(degree, result);
        m_buffer >>= 1;
        return result;
    }


    template <typename Iter1, typename Iter2>
        requires detail::iter_is_byte<Iter1>
    auto scramble_range(Iter1 first, Iter1 last, Iter2 d_first) noexcept -> void
    {
        for (; first != last; ++first, ++d_first) {
            *d_first = scramble_byte(*first);
        }
    }


    template <typename Iter1, typename Iter2>
        requires detail::iter_is_byte<Iter1>
    auto descramble_range(Iter1 first, Iter1 last, Iter2 d_first) noexcept -> void
    {
        for (; first != last; ++first, ++d_first) {
            *d_first = descramble_byte(*first);
        }
    }

private:
    buffer_type     m_buffer;
    dependency_list m_deps_descramble;
    dependency_list m_deps_scramble;
};




} // namespace lfsr