#pragma once

#include <unordered_map>
#include <vector>
#include <string>

#include "Vec2f.h"
#include "Vec3f.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"

#include "Image.h"

#include "PerlinNoise.h"

typedef const unsigned int LOD_LEVEL;

struct TerrainVertex {
	Vec3f position;
	Vec2f texcoord;
	Vec3f normal;
};

class TerrainMesh {
public:
	TerrainMesh(unsigned int gridX, unsigned int gridY, unsigned int length);
	~TerrainMesh();

public:
	static LOD_LEVEL LOD_LEVEL_01 = 1;
	static LOD_LEVEL LOD_LEVEL_02 = 2;
	static LOD_LEVEL LOD_LEVEL_03 = 3;
	static LOD_LEVEL LOD_LEVEL_04 = 4;
	static LOD_LEVEL LOD_LEVEL_05 = 5;
	static LOD_LEVEL LOD_LEVEL_06 = 6;
	static LOD_LEVEL LOD_LEVEL_07 = 7;
	static LOD_LEVEL LOD_LEVEL_08 = 8;

public:

	static TerrainMesh generateMesh(unsigned int gridX, unsigned int gridY, unsigned int length, float heightScale, LOD_LEVEL lod);

	const VertexBufferPtr& getVertexBuffer();
	const IndexBufferPtr& getIndexBuffer();

	float getHeightAt(float x, float z);
	Vec2f getGridDimensions();

private:
	void createMesh(float scale, float resolution);

	float barryCentric(const Vec3f& p1, const Vec3f& p2, const Vec3f& p3, float x, float z) {
		float det = (p2.z - p3.z) * (p1.x - p3.x) + (p3.x - p2.x) * (p1.z - p3.z);
		float l1 = ((p2.z - p3.z) * (x - p3.x) + (p3.x - p2.x) * (z - p3.z)) / det;
		float l2 = ((p3.z - p1.z) * (x - p3.x) + (p1.x - p3.x) * (z - p3.z)) / det;
		float l3 = 1.0f - l1 - l2;
		return l1 * p1.y + l2 * p2.y + l3 * p3.y;
	}

private:
	unsigned int gridX = 0;
	unsigned int gridY = 0;
	unsigned int length = 0;

	float** terrainHeights = nullptr;

	VertexBufferPtr vertexBuffer = nullptr;
	IndexBufferPtr indexBuffer = nullptr;

	float scale = 0;
	Image heightmap{};


};

