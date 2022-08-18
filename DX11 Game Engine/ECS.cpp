#include "ECS.h"

Coordinator coordinator;

void ECS::init() {
    coordinator.init();

    coordinator.registerComponent<TransformComponent>();
    coordinator.registerComponent<DrawableComponent>();
    coordinator.registerComponent<LightingComponent>();
    coordinator.registerComponent<PointLightComponent>();
    coordinator.registerComponent<PickableComponent>();

    transformSystem = coordinator.registerSystem<TransformSystem>();
    {
        Signature signature;
        signature.set(coordinator.getComponentType<TransformComponent>());
        signature.set(coordinator.getComponentType<DrawableComponent>());
        coordinator.setSystemSignature<TransformSystem>(signature);
    }

    drawableSystem = coordinator.registerSystem<DrawableSystem>();
    {
        Signature signature;
        signature.set(coordinator.getComponentType<DrawableComponent>());
        coordinator.setSystemSignature<DrawableSystem>(signature);
    }

    lightingSystem = coordinator.registerSystem<LightingSystem>();
    {
        Signature signature;
        signature.set(coordinator.getComponentType<LightingComponent>());
        signature.set(coordinator.getComponentType<DrawableComponent>());
        coordinator.setSystemSignature<LightingSystem>(signature);
    }

    pointLightSystem = coordinator.registerSystem<PointLightSystem>();
    {
        Signature signature;
        signature.set(coordinator.getComponentType<PointLightComponent>());
        signature.set(coordinator.getComponentType<TransformComponent>());
        signature.set(coordinator.getComponentType<DrawableComponent>());
        coordinator.setSystemSignature<PointLightSystem>(signature);
    }

    pickableSystem = coordinator.registerSystem<PickableSystem>();
    {
        Signature signature;
        signature.set(coordinator.getComponentType<TransformComponent>());
        signature.set(coordinator.getComponentType<PickableComponent>());
        coordinator.setSystemSignature<PickableSystem>(signature);
    }

    entities.resize(MAX_ENTITIES);
}

void ECS::update(long double deltaTime) {
    if (!systemInit) {
        transformSystem->init(deltaTime);
        systemInit = true;
    }

    transformSystem->update(deltaTime);
    lightingSystem->update();
    pointLightSystem->update();
    pickableSystem->update();
}

void ECS::draw() {
    drawableSystem->render();
}

EntityID ECS::createEntity() {
    return coordinator.createEntity();
}

void ECS::createEntity(EntityInfo entityInfo) {
    EntityID entity = coordinator.createEntity();
    coordinator.addComponent(entity, entityInfo.transform);
    coordinator.addComponent(entity, entityInfo.drawable);
}
