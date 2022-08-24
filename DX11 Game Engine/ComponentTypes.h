#pragma once

#include <iostream>
#include <unordered_map>

#include "Prerequisites.h"
#include "ConstantBufferTypes.h"

#include "Mesh.h"

#include "Vec2f.h"
#include "Vec3f.h"
#include "Vec4f.h"
#include "Mat4f.h"

#include "Camera.h"

//////////////////////////////////////////////////
// SECTION - Player Perspective and Interaction //
//////////////////////////////////////////////////


// Allows an entity to have a view of the world space.
struct CameraComponent {
	Camera camera;
};

// Allows an entity to be controlled by the player.
struct PlayableComponent {
	// needs keyboard and mouse events
	Vec3f g;
};

// Allows an entity to be mouse picked
struct PickableComponent {
};


////////////////////////////////////
// SECTION - Placement and Motion //
////////////////////////////////////


// Handles entity data with respect to position, rotation, and scale.
struct TransformComponent {
	Vec3f position{};
	Vec3f rotation{};
	Vec3f scale{};
	Camera* camera;

	// If attachedToCamera flag is true,
	// uses transform values above as offsets from camera.
	bool attachedToCamera = false;
	Vec3f translateWithCameraXYZ{};
	Vec3f rotateWithCameraXYZ{};
};


//////////////////////////////
// SECTION - Visual Aspects //
//////////////////////////////


// Handles entity data with respect to rendering.
struct DrawableComponent {
public:
	void addBuffer(CONSTANT_BUFFER_SLOT slot, ConstantBufferPtr ptr) {
		slotsInUse.push_back(slot);
		constantBufferPtrs[slot] = ptr;
		constantBufferData[slot] = nullptr;
	}

	template<typename T>
	void updateBuffer(CONSTANT_BUFFER_SLOT slot, const T& data) {
		constantBufferData[slot] = data;
	}

public:
	VertexShaderPtr vertexShader;
	PixelShaderPtr pixelShader;
	PixelShaderPtr boundingboxShader;

	std::vector<CONSTANT_BUFFER_SLOT> slotsInUse;
	ConstantBufferPtr constantBufferPtrs[SLOT_COUNT];
	void* constantBufferData[SLOT_COUNT];

	MeshPtr mesh;

	std::vector <TexturePtr> vertexShaderTextures;
	std::vector <TexturePtr> pixelShaderTextures;
};

// Handles entity data with respect to directional lighting.
struct LightingComponent {
	Vec4f lightDirection;
	Vec4f cameraPosition;
	float ambient;
	float diffuse;
	float specular;
	float shininess;
};

// Handles entity data with respect to point lighting.
struct PointLightComponent {
	Vec4f lightPosition;
	Vec4f cameraPosition;
	float radius;
};

//////////////////////////////////////
// SECTION - Physics and Collisions //
//////////////////////////////////////

struct BoundingBoxComponent {
	Vec3f position;
	Vec3f scale;
};

struct RigidbodyComponent {
	Vec3f velocity;
	Vec3f acceleration;
};
