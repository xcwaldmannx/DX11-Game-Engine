#pragma once

#include "Types.h"

#include<array>
#include <queue>
#include <assert.h>

class EntityManager {

public:
    EntityManager() {
        for (EntityID entity = 0; entity < MAX_ENTITIES; entity++) {
            availableEntities.push(entity);
        }
    }

    EntityID createEntity() {
        assert(livingEntityCount < MAX_ENTITIES && "Max number of entities exceeded.");
        EntityID id = availableEntities.front();
        availableEntities.pop();
        livingEntityCount++;
        return id;
    }

    void destroyEntity(EntityID entity) {
        assert(entity < MAX_ENTITIES && "EntityID id exceeds maximum limit.");
        signatures[entity].reset();
        availableEntities.push(entity);
        livingEntityCount--;
    }

    void setSignature(EntityID entity, Signature signature) {
        assert(entity < MAX_ENTITIES && "EntityID id exceeds maximum limit.");
        signatures[entity] = signature;
    }

    Signature getSignature(EntityID entity) {
        assert(entity < MAX_ENTITIES && "EntityID id exceeds maximum limit.");
        return signatures[entity];
    }

private:
    std::queue<EntityID> availableEntities{};
    std::array<Signature, MAX_ENTITIES> signatures{};
    std::uint16_t livingEntityCount{};

};