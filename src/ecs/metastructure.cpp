#pragma once

#include "type_traits.hpp"

template <class... Ts>
class MetaStruct {
    unique_tuple<Ts...> m_tuple;

public:
    template <class T>
    constexpr T& get() {
        return std::get<T>(m_tuple);
    }
    template <class T>
    constexpr const T& get() const {
        return std::get<T>(m_tuple);
    }
};
