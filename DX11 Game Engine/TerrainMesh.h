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

struct Terrain {
	unsigned int width = 0;
	unsigned int height = 0;
	float** terrainHeights = nullptr;

	VertexBufferPtr vertexBuffer = nullptr;
	IndexBufferPtr indexBuffer = nullptr;

	Terrain(unsigned int width, unsigned int height) : width(width), height(height) {
		// memory allocation
		terrainHeights = new float* [width];
		for (unsigned int i = 0; i < width; i++) {
			terrainHeights[i] = new float[height];
		}
	}
};

class TerrainMesh {
public:
	TerrainMesh();
	~TerrainMesh();

	const VertexBufferPtr& getVertexBuffer();
	const IndexBufferPtr& getIndexBuffer();

	static TerrainMesh generateMesh(unsigned int gridX, unsigned int gridY, unsigned int length, float scale, LOD_LEVEL lod);

	void generateFromHeightmap(const std::string& filename, float scale, float resolution);
	void generateFromPerlinNoise(float scale, float resolution);
	float getHeightAt(float x, float z);
	Vec2f getGridDimensions();

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
	void createMesh(float scale, float resolution);
	Vec3f cross(Vec3f v1, Vec3f v2);
	Vec3f normalize(Vec3f vec);

	float barryCentric(const Vec3f& p1, const Vec3f& p2, const Vec3f& p3, float x, float z) {
		float det = (p2.z - p3.z) * (p1.x - p3.x) + (p3.x - p2.x) * (p1.z - p3.z);
		float l1 = ((p2.z - p3.z) * (x - p3.x) + (p3.x - p2.x) * (z - p3.z)) / det;
		float l2 = ((p3.z - p1.z) * (x - p3.x) + (p1.x - p3.x) * (z - p3.z)) / det;
		float l3 = 1.0f - l1 - l2;
		return l1 * p1.y + l2 * p2.y + l3 * p3.y;
	}

private:
	unsigned int width = 0;
	unsigned int height = 0;
	float** terrainHeights = nullptr;

	VertexBufferPtr vertexBuffer = nullptr;
	IndexBufferPtr indexBuffer = nullptr;

	float scale = 0;
	Image heightmap{};


};

