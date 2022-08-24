#include "Terrain.h"

#include "GraphicsEngine.h"
#include "PerlinNoise.h"

Terrain::Terrain() : gridX(0), gridY(0), length(0) {
}

Terrain::Terrain(unsigned int gridX, unsigned int gridY, unsigned int length, LOD_LEVEL lod) : gridX(gridX), gridY(gridY), length(length) {
	float lodLength = length * lod;

	// allocate memory for height data
	terrainHeights = new float* [lodLength];
	for (unsigned int i = 0; i < lodLength; i++) {
		terrainHeights[i] = new float[lodLength];
	}

	// random seed values between 0-1
	float** seed = PerlinNoise::generateSeed2D(lodLength, 256.0f, true, nullptr);

	// perlin noise values between 0-1
	terrainHeights = PerlinNoise::generateNoise2D(lodLength, seed, 10, nullptr);

	// vertex data for vertex buffer
	vertices.reserve(lodLength * lodLength);

	// grid offsets for vertex positions
	float gridOffsetX = gridX * (length)-((float)gridX / (float)lod);
	float gridOffsetY = gridY * (length)-((float)gridY / (float)lod);

	for (unsigned int x = 0; x < lodLength; x++) {

		for (unsigned int z = 0; z < lodLength; z++) {

			// vertex
			float vx = (float)x / (float)lod + gridOffsetX;
			float vy = terrainHeights[x][z];
			float vz = (float)z / (float)lod + gridOffsetY;

			// texcoord
			float tx = (float)x / (float)lod;
			float ty = (float)z / (float)lod;

			// normal
			Vec3f normal{ 0, 0, 0 };
			if (x == 0) {

			}

			if (x > 0 && x < lodLength - 1 && z > 0 && z < lodLength - 1) {
				float heightL = terrainHeights[x - 1][z];
				float heightR = terrainHeights[x + 1][z];
				float heightU = terrainHeights[x][z + 1];
				float heightD = terrainHeights[x][z - 1];

				normal = Vec3f(heightL - heightR, 2.0f, heightD - heightU).normalize();
			}
			else {
				normal = Vec3f(0, 1, 0);
			}

			TerrainMesh vertex{};
			vertex.position = Vec3f(vx, vy, vz);
			vertex.texcoord = Vec2f(tx, ty);
			vertex.normal = normal;

			vertices.push_back(vertex);
		}
	}

	// index data for index buffer
	unsigned int numIndices = 6 * ((lodLength - 1) * (lodLength - 1));
	indices.reserve(numIndices);

	unsigned int currentIndex = 0;
	unsigned int step = 0;
	while (indices.size() < numIndices) {
		indices.push_back(currentIndex);
		indices.push_back(currentIndex + 1);
		indices.push_back(currentIndex + lodLength + 1);

		indices.push_back(currentIndex + lodLength + 1);
		indices.push_back(currentIndex + lodLength);
		indices.push_back(currentIndex);
		currentIndex++;
		step++;
		if (step == lodLength - 1) {
			step = 0;
			currentIndex++;
		}
	}

	createBuffers();

}

Terrain::~Terrain() {
}

void Terrain::createBuffers() {
	void* shaderByteCode = nullptr;
	size_t sizeShaderByteCode = 0;

	GraphicsEngine::get()->getRenderSystem()->compileVertexShader(L"VertexMeshLayoutShader.hlsl", "main", &shaderByteCode, &sizeShaderByteCode);
	vertexBuffer = GraphicsEngine::get()->getRenderSystem()->createVertexBuffer(&vertices[0], sizeof(TerrainMesh), (UINT)vertices.size(), (UINT*)shaderByteCode, (UINT)sizeShaderByteCode);
	indexBuffer = GraphicsEngine::get()->getRenderSystem()->createIndexBuffer(&indices[0], (UINT)indices.size());
}

VertexBufferPtr Terrain::getVertexBuffer() {
	return vertexBuffer;
}

IndexBufferPtr Terrain::getIndexBuffer() {
	return indexBuffer;
}

float Terrain::getHeightAt(float x, float z) {
	// 0______1
	// |      |
	// |      |
	// |______|
	// 2      3

	// location on face of terrain, between 0 and 1
	float faceX = -1;
	float faceZ = -1;
	float whole;

	if (x == 1) {
		faceX = 1;
	}
	else if (x == 0) {
		faceX = 0;
	}
	else {
		faceX = std::modf(x, &whole);
	}

	if (z == 1) {
		faceZ = 1;
	}
	else if (z == 0) {
		faceZ = 0;
	}
	else {
		faceZ = std::modf(z, &whole);
	}

	if (faceX <= 1 - faceZ) {
		if (x > 0 && z > 0 && (x >= length - 1 || z >= length - 1)) {
			Vec3f v0 = Vec3f(0, terrainHeights[(int)x][(int)z], 0);
			Vec3f v1 = Vec3f(1, terrainHeights[(int)x - 1][(int)z], 0);
			Vec3f v2 = Vec3f(0, terrainHeights[(int)x][(int)z - 1], 1);
			return barryCentric(v0, v1, v2, faceX, faceZ);
		}
		Vec3f v0 = Vec3f(0, terrainHeights[(int)x][(int)z], 0);
		Vec3f v1 = Vec3f(1, terrainHeights[(int)x + 1][(int)z], 0);
		Vec3f v2 = Vec3f(0, terrainHeights[(int)x][(int)z + 1], 1);
		return barryCentric(v0, v1, v2, faceX, faceZ);
	}
	else {
		if (x > 0 && z > 0 && (x >= length - 1 || z >= length - 1)) {
			Vec3f v1 = Vec3f(1, terrainHeights[(int)x - 1][(int)z], 0);
			Vec3f v2 = Vec3f(1, terrainHeights[(int)x - 1][(int)z - 1], 1);
			Vec3f v3 = Vec3f(0, terrainHeights[(int)x][(int)z - 1], 1);
			return barryCentric(v1, v2, v3, faceX, faceZ);
		}
		Vec3f v1 = Vec3f(1, terrainHeights[(int)x + 1][(int)z], 0);
		Vec3f v2 = Vec3f(1, terrainHeights[(int)x + 1][(int)z + 1], 1);
		Vec3f v3 = Vec3f(0, terrainHeights[(int)x][(int)z + 1], 1);
		return barryCentric(v1, v2, v3, faceX, faceZ);
	}

	return 0;
}
