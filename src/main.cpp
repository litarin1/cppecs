#include <log.cpp>
#include <registry.cpp>

#include "components_registry.hpp"

namespace components {
    struct A {};
    struct B {};
    struct PositionComponent {
        float x, y;
    };
}  // namespace components

using namespace components;

Entity add_dummy_entity(Registry& r) {
    Entity e = r.add_entity();
    r.emplace_component<PositionComponent>(e, 0.0f, 0.0f);
    return e;
}

int main() {
    _init_log();

    Registry r{};
    Entity entity001 = r.add_entity();
    {
        auto& tmp = r.emplace_component<PositionComponent>(entity001, 3.0f, 15.0f);
        tmp.x += 1.0f;
    }
    auto entity001_pos = r.get_component<PositionComponent>(entity001);
    if (entity001_pos) LDEBUG("x{:.2f} y{:.2f}", entity001_pos->x, entity001_pos->y);

    // r.get_pool<PositionComponent>()._debug_dump();
    LINFO("query:");
    {
        for (auto pair : r.view<PositionComponent, Exclude<A>>()) {
            auto& c = pair.get<PositionComponent>();

            LINFO("  id{}: x{:.2f} y{:.2f}", pair.e, c.x, c.y);
        }
    }
    LINFO("added component A");
    r.emplace_component<A>(entity001);
    LINFO("query:");
    {
        for (auto pair : r.view<PositionComponent, Exclude<A>>()) {
            auto& c = pair.get<PositionComponent>();

            LINFO("  id{}: x{:.2f} y{:.2f}", pair.e, c.x, c.y);
        }
    }
}
