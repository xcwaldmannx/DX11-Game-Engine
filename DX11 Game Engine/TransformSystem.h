#pragma once

#include <vector>

#include "Coordinator.h"
#include "System.h"
#include "ComponentTypes.h"

class TransformSystem : public System {
public:
    void init(long double deltaTime);
    void update(long double deltaTime);

private:
    void calculateLocation(TransformComponent& transform, DrawableComponent& drawable);
};
