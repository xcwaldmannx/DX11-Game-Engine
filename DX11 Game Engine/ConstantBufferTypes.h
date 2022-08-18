#pragma once

#include "Vec2f.h"
#include "Vec3f.h"
#include "Vec4f.h"

#include "Mat4f.h"

typedef unsigned int CONSTANT_BUFFER_SLOT;

// START SLOT DEFINITIONS

// Slots that are used to bind a constant buffer with shaders
const static CONSTANT_BUFFER_SLOT TRANSFORM   = 0;
const static CONSTANT_BUFFER_SLOT LIGHTING    = 1;
const static CONSTANT_BUFFER_SLOT POINT_LIGHT = 2;

const static CONSTANT_BUFFER_SLOT slots[] = {
	TRANSFORM,
	LIGHTING,
	POINT_LIGHT,
};

// The number of slots being used
const static unsigned int SLOT_COUNT = sizeof(slots) / sizeof(CONSTANT_BUFFER_SLOT);

// END SLOT DEFINITIONS

__declspec(align(16))
struct TransformBuffer {
	Mat4f world{};
	Mat4f view{};
	Mat4f proj{};
};

__declspec(align(16))
struct LightingBuffer {
	Vec4f lightDirection{};
	Vec4f cameraPosition{};
	float ambient = 0.0f;
	float diffuse = 0.0f;
	float specular = 0.0f;
	float shininess = 0.0f;
};

__declspec(align(16))
struct PointLightBuffer {
	Vec4f lightPosition{};
	Vec4f cameraPosition{};
	float radius = 0.0f;
};