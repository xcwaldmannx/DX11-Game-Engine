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

struct TransformComponent {
	Vec3f Position;
	Vec3f Rotation;
	Vec3f Scale;
	Camera* Camera;
	Vec3f TranslateWithCameraXYZ;
	Vec3f RotateWithCameraXYZ;
};

struct DrawableComponent {
public:
	void addBuffer(CONSTANT_BUFFER_SLOT slot, ConstantBufferPtr ptr) {
		SlotsInUse.push_back(slot);
		constantBufferPtrs[slot] = ptr;
		constantBufferData[slot] = nullptr;
	}

	template<typename T>
	void updateBuffer(CONSTANT_BUFFER_SLOT slot, const T& data) {
		constantBufferData[slot] = data;
	}

public:
	VertexShaderPtr VertexShader;
	PixelShaderPtr PixelShader;
	PixelShaderPtr BoundingBoxShader;

	std::vector<CONSTANT_BUFFER_SLOT> SlotsInUse;
	ConstantBufferPtr constantBufferPtrs[SLOT_COUNT];
	void* constantBufferData[SLOT_COUNT];

	MeshPtr Mesh;

	std::vector <TexturePtr> VertexShaderTextures;
	std::vector <TexturePtr> PixelShaderTextures;
};

struct LightingComponent {
	Vec4f LightDirection;
	Vec4f CameraPosition;
	float Ambient;
	float Diffuse;
	float Specular;
	float Shininess;
};

struct PointLightComponent {
	Vec4f LightPosition;
	Vec4f CameraPosition;
	float Radius;
};

struct PickableComponent {
};

struct BoundingBoxComponent {
	Vec3f Position;
	Vec3f Scale;
};
