#include "PickableSystem.h"

#include "Coordinator.h"
#include "ComponentTypes.h"

extern Coordinator coordinator;

void PickableSystem::update() {
	for (auto const& entity : entities) {
		if (entity) {
			auto& transform = coordinator.getComponent<TransformComponent>(entity);
			float distanceToEntity = (transform.Position - transform.Camera->getWorld().getTranslation()).magnitude();
			Vec3f directionFromCameraToEntity = (transform.Position - transform.Camera->getWorld().getTranslation()).normalize();
			Vec3f difference = (directionFromCameraToEntity)-transform.Camera->getWorld().getZDirection();


			if (difference.magnitude() <= 1 && distanceToEntity <= 3) {
				coordinator.destroyEntity(entity);
					return;
			}
		}
	}
}
