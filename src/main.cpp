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
    {
        for (const Entity e : r.view<PositionComponent, Exclude<A>>()) {
            auto c = r.get_component<PositionComponent>(e);
            if (!c) {
                LERR("wtf why c == nullptr???");
                continue;
            }

            LINFO("id{}: x{:.2f} y{:.2f}", e, c->x, c->y);
        }
    }
    r.emplace_component<A>(entity001);
    {
        for (const Entity e : r.view<PositionComponent, Exclude<A>>()) {
            auto c = r.get_component<PositionComponent>(e);
            if (!c) {
                LERR("wtf why c == nullptr???");
                continue;
            }

            LINFO("{}: x{:.2f} y{:.2f}", e, c->x, c->y);
        }
    }
}
