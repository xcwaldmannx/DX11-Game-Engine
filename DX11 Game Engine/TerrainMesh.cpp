#include "TerrainMesh.h"

#include <vector>
#include <cmath>

#include "GraphicsEngine.h"

TerrainMesh::TerrainMesh() {
}

TerrainMesh::~TerrainMesh() {
}

const VertexBufferPtr& TerrainMesh::getVertexBuffer() {
	return vertexBuffer;
}

const IndexBufferPtr& TerrainMesh::getIndexBuffer() {
	return indexBuffer;
}

TerrainMesh TerrainMesh::generateMesh(unsigned int gridX, unsigned int gridY, unsigned int length, float scale, LOD_LEVEL lod) {

	TerrainMesh tm;

	float lodLength = length * lod;

	// allocate memory for height data
	tm.terrainHeights = new float* [lodLength];
	for (unsigned int i = 0; i < lodLength; i++) {
		tm.terrainHeights[i] = new float[lodLength];
	}

	// random seed values between 0-1
	float** seed = PerlinNoise::generateSeed2D(lodLength, true, nullptr);

	// perlin noise values between 0-1
	float** noise = PerlinNoise::generateNoise2D(lodLength, seed, 8, nullptr);

	// vertex data for vertex buffer
	std::vector<TerrainVertex> vertices;
	vertices.reserve(lodLength * lodLength);

	// grid offsets for vertex positions
	float gridOffsetX = gridX * (length) - ((float) gridX / (float) lod);
	float gridOffsetY = gridY * (length) - ((float) gridY / (float) lod);

	for (unsigned int x = 0; x < lodLength; x++) {

		for (unsigned int z = 0; z < lodLength; z++) {

			// height
			tm.terrainHeights[x][z] = noise[x][z] * scale;

			// vertex
			float vx = (float) x / (float) lod + gridOffsetX;
			float vy = tm.terrainHeights[x][z];
			float vz = (float) z / (float) lod + gridOffsetY;

			// texcoord
			float tx = (float) x / (float) lod;
			float ty = (float) z / (float) lod;

			// normal
			Vec3f normal{0, 0, 0};
			if (x == 0) {

			}

			if (x > 0 && x < lodLength - 1 && z > 0 && z < lodLength - 1) {
				float heightL = noise[x - 1][z] * scale;
				float heightR = noise[x + 1][z] * scale;
				float heightU = noise[x][z + 1] * scale;
				float heightD = noise[x][z - 1] * scale;

				normal = Vec3f(heightL - heightR, 1.5f, heightD - heightU).normalize();
			} else {
				normal = Vec3f(0, 1, 0);
			}

			TerrainVertex tv{};
			tv.position = Vec3f(vx, vy, vz);
			tv.texcoord = Vec2f(tx, ty);
			tv.normal = normal;

			vertices.push_back(tv);
		}
	}

	// index data for index buffer
	std::vector<unsigned int> indices;
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

	void* shaderByteCode = nullptr;
	size_t sizeShaderByteCode = 0;

	GraphicsEngine::get()->getVertexMeshLayoutShaderByteCodeAndSize(&shaderByteCode, &sizeShaderByteCode);
	tm.vertexBuffer = GraphicsEngine::get()->getRenderSystem()->createVertexBuffer(&vertices[0], sizeof(TerrainVertex), (UINT)vertices.size(), shaderByteCode, (UINT)sizeShaderByteCode);
	tm.indexBuffer = GraphicsEngine::get()->getRenderSystem()->createIndexBuffer(&indices[0], (UINT)indices.size());

	return tm;
}

void TerrainMesh::generateFromHeightmap(const std::string& filename, float scale, float resolution) {
	heightmap.loadImage(filename);

	width = heightmap.getWidth();
	height = heightmap.getHeight();

	createMesh(scale, resolution);

}

void TerrainMesh::generateFromPerlinNoise(float scale, float resolution) {
	float** seed = PerlinNoise::generateSeed2D(width, true, nullptr);
	float** noise = PerlinNoise::generateNoise2D(width, seed, 11, nullptr);

	terrainHeights = new float* [width];
	for (unsigned int i = 0; i < width; i++) {
		terrainHeights[i] = new float[height];
	}

	std::vector<TerrainVertex> vertices;
	vertices.reserve(width * height);


	for (unsigned int x = 0; x < width; x++) {

		for (unsigned int z = 0; z < height; z++) {
			float vx = (float)x / resolution;
			float vy = noise[x][z] * scale;
			float vz = (float)z / resolution;

			float tx = (float)x;
			float ty = (float)z;

			TerrainVertex tv = {
				Vec3f(vx, vy, vz),
				Vec2f(tx, ty),
				Vec3f(0.0f, 0.0f, 0.0f)
			};

			vertices.push_back(tv);
			terrainHeights[x][z] = vy;
		}
	}

	std::vector<unsigned int> indices;
	unsigned int numIndices = 6 * ((width - 1) * (height - 1));
	indices.reserve(numIndices);

	unsigned int currentIndex = 0;
	unsigned int step = 0;
	while (indices.size() < numIndices) {
		indices.push_back(currentIndex);
		indices.push_back(currentIndex + 1);
		indices.push_back(currentIndex + width + 1);

		indices.push_back(currentIndex + width + 1);
		indices.push_back(currentIndex + width);
		indices.push_back(currentIndex);
		currentIndex++;
		step++;
		if (step == width - 1) {
			step = 0;
			currentIndex++;
		}
	}

	for (int i = 0; i < indices.size(); i += 3) {
		Vec3f v1 = vertices[indices[i + 0]].position;
		Vec3f v2 = vertices[indices[i + 1]].position;
		Vec3f v3 = vertices[indices[i + 2]].position;

		Vec3f normal = normalize(cross(v3, v2));
		vertices[indices[i + 0]].normal = normal;
		vertices[indices[i + 1]].normal = normal;
		vertices[indices[i + 2]].normal = normal;
	}

	void* shaderByteCode = nullptr;
	size_t sizeShaderByteCode = 0;

	GraphicsEngine::get()->getVertexMeshLayoutShaderByteCodeAndSize(&shaderByteCode, &sizeShaderByteCode);
	vertexBuffer = GraphicsEngine::get()->getRenderSystem()->createVertexBuffer(&vertices[0], sizeof(TerrainVertex), (UINT)vertices.size(), shaderByteCode, (UINT)sizeShaderByteCode);
	indexBuffer = GraphicsEngine::get()->getRenderSystem()->createIndexBuffer(&indices[0], (UINT)indices.size());

}

float TerrainMesh::getHeightAt(float x, float z) {

	// 0______1
	// |      |
	// |      |
	// |______|
	// 2      3

	// terrain boundary check
	if (x >= 0 && x < width - 1 && z >= 0 && z < height - 1) {
		// location on face of terrain, between 0 and 1
		float faceX = -1;
		float faceZ = -1;
		float whole;

		if (x == 1) {
			faceX = 1;
		} else if (x == 0) {
			faceX = 0;
		} else {
			faceX = std::modf(x, &whole);
		}

		if (z == 1) {
			faceZ = 1;
		} else if (z == 0) {
			faceZ = 0;
		} else {
			faceZ = std::modf(z, &whole);
		}

		if (faceX <= 1 - faceZ) {
			Vec3f v0 = Vec3f(0, terrainHeights[(int) x][(int) z], 0);
			Vec3f v1 = Vec3f(1, terrainHeights[(int) x + 1][(int) z], 0);
			Vec3f v2 = Vec3f(0, terrainHeights[(int) x][(int) z + 1], 1);
			return barryCentric(v0, v1, v2, faceX, faceZ);
		} else {
			Vec3f v1 = Vec3f(1, terrainHeights[(int)x + 1][(int)z], 0);
			Vec3f v2 = Vec3f(1, terrainHeights[(int)x + 1][(int)z + 1], 1);
			Vec3f v3 = Vec3f(0, terrainHeights[(int)x][(int)z + 1], 1);
			return barryCentric(v1, v2, v3, faceX, faceZ);
		}

	}
	return 0;
}

Vec2f TerrainMesh::getGridDimensions() {
	return Vec2f((float) width, (float) height);
}

void TerrainMesh::createMesh(float scale, float resolution) {
	terrainHeights = new float* [width];
	for (unsigned int i = 0; i < width; i++) {
		terrainHeights[i] = new float[height];
	}

	std::vector<TerrainVertex> vertices;
	vertices.reserve(width * height);


	for (unsigned int x = 0; x < width; x++) {
		float tx = (float) x;

		for (unsigned int z = 0; z < height; z++) {

			Vec4f pixel = heightmap.getRGBA(x, z);
			float avgPixelColor = (pixel.x + pixel.y + pixel.z) / 3.0f;
			float pixelHeight = scale * avgPixelColor / 256.0f;

			float vx = (float) x / resolution;
			float vy = pixelHeight;
			float vz = (float) z / resolution;

			float ty = (float) z;

			TerrainVertex tv = {
				Vec3f(vx, vy, vz),
				Vec2f(tx, ty),
				Vec3f(0.0f, 0.0f, 0.0f)
			};

			vertices.push_back(tv);
			terrainHeights[x][z] = vy;
		}
	}

	std::vector<unsigned int> indices;
	unsigned int numIndices = 6 * ((width - 1) * (height - 1));
	indices.reserve(numIndices);

	unsigned int currentIndex = 0;
	unsigned int step = 0;
	while (indices.size() < numIndices) {
		indices.push_back(currentIndex);
		indices.push_back(currentIndex + 1);
		indices.push_back(currentIndex + width + 1);

		indices.push_back(currentIndex + width + 1);
		indices.push_back(currentIndex + width);
		indices.push_back(currentIndex);
		currentIndex++;
		step++;
		if (step == width - 1) {
			step = 0;
			currentIndex++;
		}
	}

	for (int i = 0; i < indices.size(); i += 3) {
		Vec3f v1 = vertices[indices[i + 0]].position;
		Vec3f v2 = vertices[indices[i + 1]].position;
		Vec3f v3 = vertices[indices[i + 2]].position;

		Vec3f normal = normalize(cross(v3, v2));
		vertices[indices[i + 0]].normal = normal;
		vertices[indices[i + 1]].normal = normal;
		vertices[indices[i + 2]].normal = normal;
	}

	void* shaderByteCode = nullptr;
	size_t sizeShaderByteCode = 0;

	GraphicsEngine::get()->getVertexMeshLayoutShaderByteCodeAndSize(&shaderByteCode, &sizeShaderByteCode);
	vertexBuffer = GraphicsEngine::get()->getRenderSystem()->createVertexBuffer(&vertices[0], sizeof(TerrainVertex), (UINT)vertices.size(), shaderByteCode, (UINT)sizeShaderByteCode);
	indexBuffer = GraphicsEngine::get()->getRenderSystem()->createIndexBuffer(&indices[0], (UINT)indices.size());

}

Vec3f TerrainMesh::cross(Vec3f v1, Vec3f v2) {
	float x =   (v1.y * v2.z) - (v2.y * v1.z);
	float y = -((v1.x * v2.z) - (v2.x * v1.z));
	float z =   (v1.x * v2.y) - (v2.x * v1.y);
	return Vec3f(x, y, z);
}

Vec3f TerrainMesh::normalize(Vec3f vec) {
	float len = (float) sqrt(pow(vec.x, 2) + pow(vec.y, 2) + pow(vec.z, 2));
	return Vec3f(vec.x / len, vec.y / len, vec.z / len);
}

