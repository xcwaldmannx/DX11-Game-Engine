#pragma once

#include <vector>

#include "ComponentTypes.h"

#include "Coordinator.h"

#include "TransformSystem.h"
#include "DrawableSystem.h"
#include "LightingSystem.h"
#include "PointLightSystem.h"
#include "PickableSystem.h"

extern Coordinator coordinator;

struct EntityInfo {
    TransformComponent transform{};
    DrawableComponent drawable{};
};

class ECS {
public:
    void init();
    void update(long double deltaTime);
    void draw();
    EntityID createEntity();
    void createEntity(EntityInfo entityInfo);

    template<typename T>
    void addComponentToEntity(EntityID entity, T component) {
        coordinator.addComponent<T>(entity, component);
    }

private:
    bool systemInit = false;

    std::shared_ptr<TransformSystem> transformSystem = nullptr;
    std::shared_ptr<DrawableSystem> drawableSystem = nullptr;
    std::shared_ptr<LightingSystem> lightingSystem = nullptr;
    std::shared_ptr<PointLightSystem> pointLightSystem = nullptr;
    std::shared_ptr<PickableSystem> pickableSystem = nullptr;

    std::vector<EntityID> entities{};
};
