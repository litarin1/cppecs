#pragma once

#include <tuple>
#include <type_traits>

template <class T, class... Left>
constexpr bool is_all_unique_v = (!std::is_same_v<T, Left> && ...) && is_all_unique_v<Left...>;
template <class T>
constexpr bool is_all_unique_v<T> = true;

template <class... Ts>
concept is_all_unique = (is_all_unique_v<Ts...>);

template <class... Ts>
    requires is_all_unique<Ts...>
using unique_tuple = std::tuple<Ts...>;
