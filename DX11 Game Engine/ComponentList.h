#pragma once

#include <array>
#include <unordered_map>

#include "Types.h"

class ComponentListInterface {
public:
    virtual ~ComponentListInterface() = default;
    virtual void entityDestroyed(EntityID entity) = 0;
};

template <typename T>
class ComponentList : public ComponentListInterface {
public:
    void insert(EntityID entity, T component) {
        assert(entityToIndexMap.find(entity) == entityToIndexMap.end() && "Component added to same entity more than once.");
        size_t newIndex = size;
        entityToIndexMap[entity] = newIndex;
        indexToEntityMap[newIndex] = entity;
        components[newIndex] = component;
        size++;
    }

    void remove(EntityID entity) {
        assert(entityToIndexMap.find(entity) != entityToIndexMap.end() && "Removing non-existent component.");
        size_t indexOfRemovedEntity = entityToIndexMap[entity];
        size_t indexOfLastElement = size - 1;
        components[indexOfRemovedEntity] = components[indexOfLastElement];

        EntityID entityOfLastElement = indexToEntityMap[indexOfLastElement];
        entityToIndexMap[entityOfLastElement] = indexOfRemovedEntity;
        indexToEntityMap[indexOfRemovedEntity] = entityOfLastElement;

        entityToIndexMap.erase(entity);
        indexToEntityMap.erase(indexOfLastElement);
        size--;
    }

    T& get(EntityID entity) {
        assert(entityToIndexMap.find(entity) != entityToIndexMap.end() && "Retrieving non-existent component.");
        return components[entityToIndexMap[entity]];
    }

    void entityDestroyed(EntityID entity) override {
        if (entityToIndexMap.find(entity) != entityToIndexMap.end()) {
            remove(entity);
        }
    }

private:
    std::array<T, MAX_ENTITIES> components{};
    std::unordered_map<EntityID, size_t> entityToIndexMap{};
    std::unordered_map<size_t, EntityID> indexToEntityMap{};
    size_t size{};
};
