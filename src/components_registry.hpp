#pragma once
#include <cstddef>

#include "components_list.hpp"

namespace components {
    enum class Components : size_t {
#define XCOMPONENT(name, ...) name,
        COMPONENTS_LIST
#undef XCOMPONENT
    };

    template <class Component>
    consteval size_t get_component_id() {
        static_assert(sizeof(Component) == 0, "undefined component!");
        return 0;
    }

#define XCOMPONENT(type, ...)                         \
    struct type;                                      \
    template <>                                       \
    consteval size_t get_component_id<type>() {       \
        return static_cast<size_t>(Components::type); \
    }
    COMPONENTS_LIST
#undef XCOMPONENT
}  // namespace components
