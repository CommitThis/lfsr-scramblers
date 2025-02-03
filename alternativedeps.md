# Alternative Dependency Calculation

The following code is the original for calculating the dependencies, before it
was rewritten to replace the sets, vectors and maps with bitsets and XORs. It's
left here as it might be more demonstrative to some.

```cpp

template <typename T>
auto to_set(std::vector<T> const & vec) -> std::set<T>;

template <typename T>
auto to_vector(std::set<T> const & set) -> std::vector<T>;

template <typename K, typename V>
auto keys_for(std::map<K, V> const & map) -> std::vector<K>;

template <typename T>
auto intersection_of(std::set<T> const & lhs, std::set<T> const & rhs)
    -> std::set<T>;


template <std::size_t ... Taps>
static auto scramble_dependencies() -> dependency_list
{
    using tap_list       = detail::make_tap_list_t<Taps...>;
    using dependency_map = std::map<std::size_t, std::vector<std::size_t>>;

    auto dependencies = dependency_map{};
    auto result       = dependency_list{};

    for (auto b = 0ull; b != char_bits; ++b) 
    {
        auto input_index = b + degree;
        auto tap_mask  = buffer_type{};
        auto indices   = std::vector<std::size_t>{};

        /* Don't include the current bit in the dependencies as they are 
           already included in the dependency set (otherwise they couldn't
           be looked up) and will get annihilated by the XOR operation; and
           this dependency will not contribute to the result. */
        for (auto tap : {Taps...}) {
            if (tap == 0)
                continue;
            indices.push_back(degree - tap + b);
        }

        auto existing_deps = detail::keys_for(dependencies);
        auto shared = detail::intersection_of(
                detail::to_set(indices), 
                detail::to_set(existing_deps));
        for (auto tap : shared) {
            indices.insert(indices.end(), 
                    dependencies[tap].begin(), 
                    dependencies[tap].end());
        }

        /* If we stick in the source (data) bit here, it will be added to
           the dependency set and will also be annihilated by future mask
           operations that depend on it as it will be included twice. */
        dependencies[input_index] = indices;

        for (auto index : indices) {
            tap_mask.flip(index);
        }

        /* This is the safe place to add the input bit. */
        tap_mask.set(degree + b);

        result[b] = tap_mask;
    }

    return result;
}


template <typename T>
auto to_set(std::vector<T> const & vec) -> std::set<T>
{
    return std::set<T>{vec.begin(), vec.end()};
}

template <typename T>
auto to_vector(std::set<T> const & set) -> std::vector<T>
{
    return std::vector<T>{set.begin(), set.end()};
}

template <typename K, typename V>
auto keys_for(std::map<K, V> const & map) -> std::vector<K>
{
    auto result = std::vector<K>{};
    for (auto const & [key, value] : map) {
        result.push_back(key);
    }
    return result;
}

template <typename T>
auto intersection_of(std::set<T> const & lhs, std::set<T> const & rhs) 
    -> std::set<T>
{
    auto result = std::set<T>{};
    std::set_intersection(lhs.begin(), lhs.end(), rhs.begin(), rhs.end(), 
            std::inserter(result, result.end()));
    return result;
}


```