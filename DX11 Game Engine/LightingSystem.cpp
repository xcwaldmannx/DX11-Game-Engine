#include "LightingSystem.h"

extern Coordinator coordinator;

void LightingSystem::update() {
    for (auto const& entity : entities) {
        auto& drawable = coordinator.getComponent<DrawableComponent>(entity);
        auto& lighting = coordinator.getComponent<LightingComponent>(entity);

        LightingBuffer* lightingBuffer = new LightingBuffer{};

        // LIGHTING BUFFER
        lightingBuffer->cameraPosition = lighting.cameraPosition;
        lightingBuffer->lightDirection = lighting.lightDirection;
        lightingBuffer->ambient = lighting.ambient;
        lightingBuffer->diffuse = lighting.diffuse;
        lightingBuffer->specular = lighting.specular;
        lightingBuffer->shininess = lighting.shininess;

        drawable.updateBuffer<LightingBuffer*>(LIGHTING, lightingBuffer);
        lightingBuffer = nullptr;
    }
}
