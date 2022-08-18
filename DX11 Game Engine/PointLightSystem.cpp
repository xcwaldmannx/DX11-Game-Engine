#include "PointLightSystem.h"

#include "Coordinator.h"
#include "ComponentTypes.h"

#include "ConstantBuffer.h"
#include "ConstantBufferTypes.h"


extern Coordinator coordinator;

void PointLightSystem::update() {
	for (auto const& entity : entities) {
		auto& pointLight = coordinator.getComponent<PointLightComponent>(entity);
		auto& transform = coordinator.getComponent<TransformComponent>(entity);
		auto& drawable = coordinator.getComponent<DrawableComponent>(entity);

		PointLightBuffer* pointLightBuffer = new PointLightBuffer{};

		pointLightBuffer->lightPosition = pointLight.LightPosition;
		pointLightBuffer->cameraPosition = transform.Camera->getWorld().getTranslation();
		pointLightBuffer->radius = pointLight.Radius;

		drawable.updateBuffer<PointLightBuffer*>(POINT_LIGHT, pointLightBuffer);
		pointLightBuffer = nullptr;

	}
}