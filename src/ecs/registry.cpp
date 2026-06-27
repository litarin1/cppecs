#include <ranges>
#include <tuple>
#include <types.hpp>

#include "../components_registry.hpp" // IWYU pragma: keep
#include "../utils/tuples.hpp"
#include "log.cpp"
#include "metastructure.cpp"
#include "pool.cpp"

template <class... Ts>
struct Exclude {};

template <class... ComponentsList>
class BaseRegistry {
    MetaStruct<ComponentPool<ComponentsList>...> m_pools;

    Entity m_next_entity_id = 0;

    template <class CurrentIncludes, class CurrentExcludes, class... Left>
    struct ViewQuerySplitter;
    // base case
    template <class... Includes, class... Excludes>
    struct ViewQuerySplitter<std::tuple<Includes...>, std::tuple<Excludes...>> {
        using Include = std::tuple<Includes...>;
        using Exclude = std::tuple<Excludes...>;
    };

    // exclude case
    template <class... Includes, class... Excludes, class NextExcludes, class... Tail>
    struct ViewQuerySplitter<std::tuple<Includes...>, std::tuple<Excludes...>, Exclude<NextExcludes>, Tail...> {
        using Next = ViewQuerySplitter<std::tuple<Includes...>, std::tuple<Excludes..., NextExcludes>, Tail...>;
        using Include = typename Next::Include;
        using Exclude = typename Next::Exclude;
    };

    // include case
    template <class... Includes, class... Excludes, class Head, class... Tail>
    struct ViewQuerySplitter<std::tuple<Includes...>, std::tuple<Excludes...>, Head, Tail...> {
        using Next = ViewQuerySplitter<std::tuple<Includes..., Head>, std::tuple<Excludes...>, Tail...>;
        using Include = typename Next::Include;
        using Exclude = typename Next::Exclude;
    };

    template <class... Filters>
    using ViewQuery = ViewQuerySplitter<std::tuple<>, std::tuple<>, Filters...>;

    template <class... Filters>
    class View {
        using Query = ViewQuery<Filters...>;
        using Includes = typename Query::Include;
        using Excludes = typename Query::Exclude;

        const BaseRegistry<ComponentsList...>& m_reg;

        bool match(Entity entity) const {
            return [&]<typename... Pools>(std::tuple<Pools...>*) { (m_reg.get_pool<Pools>().has(entity) and ...); }((Includes*)nullptr);
        }
    };

public:
    // @warning reference is temporary and is valid just until any other operations on arrays. use get_component() to renew the reference
    template <class Component>
    Component& push_component(const Entity entity, Component&& component) {
        return get_pool<Component>().push(entity, std::forward<Component>(component));
    }
    // @warning reference is temporary and is valid just until any other operations on arrays. use get_component() to renew the reference
    template <class Component, class... Args>
    Component& emplace_component(const Entity entity, Args&&... args) {
        return get_pool<Component>().emplace(entity, std::forward<Args>(args)...);
    }

    Entity add_entity() {
        LDEBUG("reg: create entity {}", m_next_entity_id);
        return m_next_entity_id++;
    }
    void remove_entity(const Entity id) { (get_pool<ComponentsList>().remove_entity(id), ...); }

    template <class Component>
    constexpr ComponentPool<Component>& get_pool() {
        return m_pools.template get<ComponentPool<Component>>();
    }
    // may return nullptr
    template <class Component>
    constexpr Component* get_component(const Entity entity) {
        return get_pool<Component>().get(entity);
    }

    template <class... Filters>
    auto view() {
        using Query = ViewQuery<Filters...>;
        using Includes = typename Query::Include;
        using Excludes = typename Query::Exclude;

        std::vector<Entity>* smallest = nullptr;
        unpack_tuple_types<Includes>([&]<class... Components>() {
            (
                [&]() {
                    std::vector<Entity>* array = get_pool<Components>().get_entities();
                    if (!smallest or array->size() < smallest->size()) smallest = array;
                }(),
                ...);
        });

        auto pred = [&](const Entity entity) -> bool {
            return unpack_tuple_types<Includes>([&]<class... Components>() { return (get_pool<Components>().has(entity) && ...); }) &&
                   unpack_tuple_types<Excludes>([&]<class... Components>() { return (!get_pool<Components>().has(entity) && ...); });
        };
        if (smallest) {
            auto view = (*smallest) | std::views::filter(pred);
            return view;
        } else {
            static std::vector<Entity> dummy_empty{};
            return dummy_empty | std::views::filter(pred);
        }
    }
};

#define SEP ,
using Registry = BaseRegistry<
#define XCOMPONENT(type, sep) components::type sep
    COMPONENTS_LIST
#undef XCOMPONENT
    >;
#undef SEP
