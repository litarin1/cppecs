#include <tuple>

#include "log.cpp"

// template <class Tuple, class Func>
// constexpr void for_each_tuple_type(Func&& func) {
//     constexpr auto size = std::tuple_size_v<Tuple>;

//     [&]<auto... Is>(std::index_sequence<Is...>) { (func.template operator()<std::tuple_element_t<Is, Tuple>>(), ...); }(std::make_index_sequence<size>{});
// }

// forward declaration
template <class T>
struct unpack_tuple_and_run;

template <class... Ts>
struct unpack_tuple_and_run<std::tuple<Ts...>> {
    template <typename Func>
    constexpr static auto apply(Func&& func) {
        return Func(func).template operator()<Ts...>();
    }
};
template <class Tuple, class Func>
constexpr auto unpack_tuple_types(Func&& func) {
    return unpack_tuple_and_run<Tuple>::apply(func);
}
