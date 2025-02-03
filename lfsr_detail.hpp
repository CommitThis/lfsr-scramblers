#pragma once

#include <cstddef>
#include <cstdint>
#include <map>
#include <set>
#include <vector>

#include <lfsr_tap_list.hpp>

namespace lfsr::detail {


template <typename Iter>
using iter_value_t = typename std::iterator_traits<Iter>::value_type;

template <typename Iter>
constexpr static auto iter_is_byte 
    = std::is_same_v<iter_value_t<Iter>, std::uint8_t>;

template <typename Iter>
constexpr static auto iter_is_bool = std::is_same_v<iter_value_t<Iter>, bool>;



/* We want to ignore any tap that is zero valued -- this just represents the
   input bit and does not need to be iterated over in the stored bufffer of
   bits. The idea is that `TapList` holds a type that is some variant of
   `tap_list`. Initially, this would be `tap_list<>`, but this will be built
   up as the template is recursively instantiated, by being rebound as follows
   `tap_list<...>::add<NewTap>` => `tap_list<..., NewTap>` 
*/
template <typename TapList, std::size_t ToRemove, std::size_t ... Taps>
struct remove_tap;

/* Helper so we dont have to type `typename` and `value_type` everywhere */
template <typename TapList, std::size_t ToRemove, std::size_t ... Taps>
using remove_tap_t = 
    typename remove_tap<TapList, ToRemove, Taps...>::value_type;


template <typename TapList, std::size_t ToRemove>
struct remove_tap<TapList, ToRemove>
{
    using value_type = TapList;
};

template <typename TapList, std::size_t ToRemove, std::size_t ThisTap, std::size_t ... OtherTaps>
struct remove_tap<TapList, ToRemove, ThisTap, OtherTaps...>
{
    using new_list = typename TapList::template add<ThisTap>;
    using value_type =
        std::conditional_t<ThisTap != ToRemove,
            remove_tap_t<new_list, ToRemove, OtherTaps...>,
            remove_tap_t<TapList,  ToRemove, OtherTaps...>
        >;
};

template <std::size_t ... Taps>
using make_tap_list_t = typename remove_tap<tap_list<>, 0, Taps...>::value_type;

template <std::size_t ... Taps>
using make_index_list_t = typename make_tap_list_t<Taps...>::to_indices;

}

