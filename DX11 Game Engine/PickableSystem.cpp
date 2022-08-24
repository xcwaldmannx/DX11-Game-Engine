#include "PickableSystem.h"

extern Coordinator coordinator;

void PickableSystem::update() {
	for (auto const& entity : entities) {
		if (entity) {
			auto& transform = coordinator.getComponent<TransformComponent>(entity);
			float distanceToEntity = (transform.position - transform.camera->getWorld().getTranslation()).magnitude();
			Vec3f directionFromCameraToEntity = (transform.position - transform.camera->getWorld().getTranslation()).normalize();
			Vec3f difference = (directionFromCameraToEntity)-transform.camera->getWorld().getZDirection();


			if (difference.magnitude() <= 1 && distanceToEntity <= 3) {
				coordinator.destroyEntity(entity);
					return;
			}
		}
	}
}
