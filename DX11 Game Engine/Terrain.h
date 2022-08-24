#pragma once

#include <vector>

#include "VertexBuffer.h"
#include "IndexBuffer.h"

#include "Vec2f.h"
#include "Vec3f.h"

typedef const unsigned int LOD_LEVEL;

struct TerrainMesh {
	Vec3f position;
	Vec2f texcoord;
	Vec3f normal;
};

class Terrain {
public:
	Terrain();
	Terrain(unsigned int gridX, unsigned int gridY, unsigned int length, LOD_LEVEL lod);
	~Terrain();

	void createBuffers();

	VertexBufferPtr getVertexBuffer();
	IndexBufferPtr getIndexBuffer();

	float getHeightAt(float x, float z);

private:
	float barryCentric(const Vec3f& p1, const Vec3f& p2, const Vec3f& p3, float x, float z) {
		float det = (p2.z - p3.z) * (p1.x - p3.x) + (p3.x - p2.x) * (p1.z - p3.z);
		float l1 = ((p2.z - p3.z) * (x - p3.x) + (p3.x - p2.x) * (z - p3.z)) / det;
		float l2 = ((p3.z - p1.z) * (x - p3.x) + (p1.x - p3.x) * (z - p3.z)) / det;
		float l3 = 1.0f - l1 - l2;
		return l1 * p1.y + l2 * p2.y + l3 * p3.y;
	}

public:
	static LOD_LEVEL LOD_LEVEL_01 = 1;
	static LOD_LEVEL LOD_LEVEL_02 = 2;
	static LOD_LEVEL LOD_LEVEL_03 = 3;
	static LOD_LEVEL LOD_LEVEL_04 = 4;
	static LOD_LEVEL LOD_LEVEL_05 = 5;
	static LOD_LEVEL LOD_LEVEL_06 = 6;
	static LOD_LEVEL LOD_LEVEL_07 = 7;
	static LOD_LEVEL LOD_LEVEL_08 = 8;

private:
	unsigned int gridX = 0;
	unsigned int gridY = 0;
	unsigned int length = 0;
	float** terrainHeights = nullptr;

	std::vector<TerrainMesh> vertices;
	std::vector<unsigned int> indices;

	VertexBufferPtr vertexBuffer = nullptr;
	IndexBufferPtr indexBuffer = nullptr;
};

