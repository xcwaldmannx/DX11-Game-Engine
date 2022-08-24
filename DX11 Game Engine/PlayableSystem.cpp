#include "PlayableSystem.h"

extern Coordinator coordinator;

void PlayableSystem::update(long double deltaTime) {
	for (const auto& entity : entities) {
		auto& playable = coordinator.getComponent<PlayableComponent>(entity);
		auto& camera = coordinator.getComponent<CameraComponent>(entity);
		auto& rigidbody = coordinator.getComponent<RigidbodyComponent>(entity);
	}
}