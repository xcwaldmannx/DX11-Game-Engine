#pragma once

#include <memory>

#include "Types.h"
#include "EntityManager.h"
#include "ComponentManager.h"
#include "SystemManager.h"

class Coordinator {
public:
    void init() {
        entityManager = std::make_unique<EntityManager>();
        componentManager = std::make_unique<ComponentManager>();
        systemManager = std::make_unique<SystemManager>();
    }

    // EntityID methods

    EntityID createEntity() {
        return entityManager->createEntity();
    }

    void destroyEntity(EntityID entity) {
        entityManager->destroyEntity(entity);
        componentManager->entityDestroyed(entity);
        systemManager->entityDestroyed(entity);
    }

    // component methods

    template<typename T> void registerComponent() {
        componentManager->registerComponent<T>();
    }

    template<typename T> void addComponent(EntityID entity, T component) {
        componentManager->addComponent<T>(entity, component);
        auto signature = entityManager->getSignature(entity);
        signature.set(componentManager->getComponentType<T>(), true);
        entityManager->setSignature(entity, signature);
        systemManager->entitySignatureChanged(entity, signature);
    }

    template<typename T> void removeComponent(EntityID entity) {
        componentManager->removeComponent<T>(entity);
        auto signature = entityManager->getSignature(entity);
        signature.set(componentManager->getComponentType<T>(), false);
        entityManager->setSignature(entity, signature);
        systemManager->entitySignatureChanged(entity, signature);
    }

    template<typename T> T& getComponent(EntityID entity) {
        return componentManager->getComponent<T>(entity);
    }

    template<typename T> ComponentType getComponentType() {
        return componentManager->getComponentType<T>();
    }

    // system methods

    template<typename T> std::shared_ptr<T> registerSystem() {
        return systemManager->registerSystem<T>();
    }

    template<typename T> void setSystemSignature(Signature signature) {
        systemManager->setSignature<T>(signature);
    }


private:
    std::unique_ptr<EntityManager> entityManager = nullptr;
    std::unique_ptr<ComponentManager> componentManager = nullptr;
    std::unique_ptr<SystemManager> systemManager = nullptr;
};