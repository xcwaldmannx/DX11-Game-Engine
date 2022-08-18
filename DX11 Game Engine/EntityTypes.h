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
		transform.Position = Vec3f(0, 0, 0);
		transform.Rotation = Vec3f(0, 0, 0);
		transform.Scale = Vec3f(1, 1, 1);

		// drawable

		// lighting
		lighting.Ambient = 0.3f;
		lighting.Diffuse = 1.0f;
		lighting.Specular = 0.0f;
		lighting.Shininess = 0.0f;
	}
};

struct EntityTree : public EntityInfo {

	LightingComponent lighting{};
	PickableComponent pickable{};

	EntityTree() {
		// transform
		transform.Position = Vec3f(0, 0, 0);
		transform.Rotation = Vec3f(0, random(0, PI), 0);
		transform.Scale = Vec3f(0.5f, 0.5f, 0.5f);

		// drawable

		// lighting
		lighting.Ambient = 0.3f;
		lighting.Diffuse = 1.0f;
		lighting.Specular = 0.0f;
		lighting.Shininess = 0.0f;
	}
};

struct EntityPointLight : public EntityInfo {

	PointLightComponent pointLight{};

	EntityPointLight() {
		pointLight.LightPosition = Vec4f(0, 1, 0, 0);
		pointLight.Radius = 1.0f;
	}
};

struct EntityPlayerHand : public EntityInfo {
};