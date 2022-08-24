#pragma once

# define PI 3.14159265358979323846f

#include "ECS.h"
#include "ComponentTypes.h"

#include "Vec3f.h"
#include "Vec4f.h"

float random(float low, float high) {
	return low + static_cast<float>(rand()) * (high - low) / RAND_MAX;
}

struct EntityTerrain : public EntityInfo {

	LightingComponent lighting{};

	EntityTerrain() {
		// transform
		transform.position = Vec3f(0, 0, 0);
		transform.rotation = Vec3f(0, 0, 0);
		transform.scale = Vec3f(1, 1, 1);

		// drawable

		// lighting
		lighting.ambient = 0.3f;
		lighting.diffuse = 1.0f;
		lighting.specular = 0.0f;
		lighting.shininess = 0.0f;
	}
};

struct EntityTree : public EntityInfo {

	LightingComponent lighting{};
	PickableComponent pickable{};

	EntityTree() {
		// transform
		transform.position = Vec3f(0, 0, 0);
		transform.rotation = Vec3f(0, random(0, PI), 0);
		transform.scale = Vec3f(0.5f, 0.5f, 0.5f);

		// drawable

		// lighting
		lighting.ambient = 0.3f;
		lighting.diffuse = 1.0f;
		lighting.specular = 0.0f;
		lighting.shininess = 0.0f;
	}
};

struct EntityPointLight : public EntityInfo {

	PointLightComponent pointLight{};

	EntityPointLight() {
		pointLight.lightPosition = Vec4f(0, 1, 0, 0);
		pointLight.radius = 1.0f;
	}
};

struct EntityPlayerHand : public EntityInfo {
};