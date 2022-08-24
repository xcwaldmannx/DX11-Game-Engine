#include "PointLightSystem.h"

extern Coordinator coordinator;

void PointLightSystem::update() {
	for (auto const& entity : entities) {
		auto& pointLight = coordinator.getComponent<PointLightComponent>(entity);
		auto& transform = coordinator.getComponent<TransformComponent>(entity);
		auto& drawable = coordinator.getComponent<DrawableComponent>(entity);

		PointLightBuffer* pointLightBuffer = new PointLightBuffer{};

		pointLightBuffer->lightPosition = pointLight.lightPosition;
		pointLightBuffer->cameraPosition = transform.camera->getWorld().getTranslation();
		pointLightBuffer->radius = pointLight.radius;

		drawable.updateBuffer<PointLightBuffer*>(POINT_LIGHT, pointLightBuffer);
		pointLightBuffer = nullptr;

	}
}