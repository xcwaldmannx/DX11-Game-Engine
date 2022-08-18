#include "LightingSystem.h"

#include "Coordinator.h"
#include "ComponentTypes.h"

#include "ConstantBuffer.h"
#include "ConstantBufferTypes.h"

extern Coordinator coordinator;

void LightingSystem::update() {
    for (auto const& entity : entities) {
        auto& drawable = coordinator.getComponent<DrawableComponent>(entity);
        auto& lighting = coordinator.getComponent<LightingComponent>(entity);

        LightingBuffer* lightingBuffer = new LightingBuffer{};

        // LIGHTING BUFFER
        lightingBuffer->cameraPosition = lighting.CameraPosition;
        lightingBuffer->lightDirection = lighting.LightDirection;
        lightingBuffer->ambient = lighting.Ambient;
        lightingBuffer->diffuse = lighting.Diffuse;
        lightingBuffer->specular = lighting.Specular;
        lightingBuffer->shininess = lighting.Shininess;

        drawable.updateBuffer<LightingBuffer*>(LIGHTING, lightingBuffer);
        lightingBuffer = nullptr;
    }
}
