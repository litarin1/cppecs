#include <cstdio>
#include <log.cpp>
#include <types.hpp>
#include <vector>

template <class TComponent>
class ComponentPool {
    // dense array of components
    std::vector<TComponent> m_components;
    // sparse array
    std::vector<size_t> m_entity_to_components;
    // dense array. same indexes as m_components.
    // used for remove()
    std::vector<Entity> m_components_to_entity;

public:
    // does not reserve sparse array
    void reserve(const size_t n) {
        m_components.reserve(n);
        m_components_to_entity.reserve(n);
    }
    void grow_sparse_to(const Entity new_max) {
        if (new_max >= m_entity_to_components.size()) m_entity_to_components.resize(new_max + 1, INVALID_COMPONENT);
    }

    // get component
    // returns nullptr if entity does not exist
    TComponent* get(const Entity entity) {
        if (entity < m_entity_to_components.size()) {
            size_t component = m_entity_to_components[entity];
            if (component != INVALID_COMPONENT) return &m_components[component];
        }
        return nullptr;
    }

    // has entity
    bool has(const Entity entity) const {
        if (entity < m_entity_to_components.size() and m_entity_to_components[entity] != INVALID_COMPONENT) return true;
        return false;
    }

    // perfect forwarding you know
    // @warning reference is temporary and is valid just until any other operations on arrays. use get() to renew the reference
    template <typename T>
    TComponent& push(const Entity entity, T&& component) {
        grow_sparse_to(entity);
        size_t idx = m_components.size();
        m_components.push_back(std::forward<T>(component));
        m_entity_to_components[entity] = idx;
        m_components_to_entity.push_back(entity);
        return m_components[idx];
    }
    // @warning reference is temporary and is valid just until any other operations on arrays. use get() to renew the reference
    template <class... Args>
    TComponent& emplace(const Entity entity, Args&&... args) {
        grow_sparse_to(entity);
        size_t idx = m_components.size();
        m_entity_to_components[entity] = idx;
        m_components_to_entity.push_back(entity);
        return m_components.emplace_back(std::forward<Args>(args)...);
    }

    void remove_entity(const Entity entity) {
        if (entity >= m_entity_to_components.size()) return;  // bound check
        size_t idx = m_entity_to_components[entity];
        if (idx == INVALID_COMPONENT) return;
        size_t last = m_components.size() - 1;
        m_entity_to_components[entity] = INVALID_COMPONENT;
        if (idx == last) {
            // just pop
            m_components.pop_back();
            m_entity_to_components[m_components_to_entity[idx]] = INVALID_COMPONENT;
            m_components_to_entity.pop_back();
        } else {
            // swap-and-pop
            m_components[idx] = m_components[last];
            m_components.pop_back();
            // update the swapped index
            m_entity_to_components[m_components_to_entity[last]] = idx;
            // remove to keep size and indexes same as m_components
            m_components_to_entity.pop_back();
        }
        LINFO("REMOVE HAPPENED");
        // _debug_dump();
        // m_components.size() - should decrease
        // m_components_to_entity.size() - should decrease
        // m_entity_to_components.size() - may not decrease
    }

    ComponentPool() {
        LDEBUG("{}", __PRETTY_FUNCTION__);
        _debug_dump();
    }

public:
    std::vector<Entity>* get_entities() { return &m_components_to_entity; }

    inline void _debug_dump() {
        LDEBUG("  m_components          : {}", m_components.size());
        LDEBUG("  m_entity_to_components: {}", m_entity_to_components.size());
        LDEBUG("  m_entity_to_components: {}", m_entity_to_components);
        LDEBUG("  m_components_to_entity: {}", m_components_to_entity.size());
    }
};
