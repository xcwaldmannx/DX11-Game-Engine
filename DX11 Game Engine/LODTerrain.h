#pragma once

#include <mutex>
#include <thread>
#include <atomic>

#include "StaticQuadTree.h"

#include "VertexBuffer.h"

#include "Vec2f.h"
#include "Vec3f.h"
#include "Vertex.h"

struct LODChunk {
	QTRect area;

	float** seed = nullptr;
	float** terrainHeights = nullptr;

	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;

	VertexBufferPtr vertexBuffer = nullptr;
	IndexBufferPtr indexBuffer = nullptr;

	bool isLoadReady = false;
	bool isRenderReady = false;
	bool isProcessing = false;

	size_t sizeVertices() {
		return vertices.size();
	}

	size_t sizeIndices() {
		return indices.size();
	}
};

class LODTerrain {
public:
	LODTerrain(unsigned int lengthX, unsigned int lengthZ, unsigned int terrainSize);
	~LODTerrain();

	void setViewArea(const QTRect& area);

	void initChunks();
	void loadInitData(LODChunk* chunk);
	void loadChunks();
	void generateChunk(LODChunk* chunk);

	std::vector<LODChunk*> getChunks(const QTRect& area);

	void draw(VertexShaderPtr vs, PixelShaderPtr ps, ConstantBufferPtr transform, ConstantBufferPtr lighting);

	float getHeightAt(float x, float z);

private:
	// most significant bit
	int msb(int n) {
		int result = 0;
		while (n >>= 1) {
			result++;
		}
		return result;
	}

	// computes height of terrain
	float barryCentric(const Vec3f& p1, const Vec3f& p2, const Vec3f& p3, float x, float z) {
		float det = (p2.z - p3.z) * (p1.x - p3.x) + (p3.x - p2.x) * (p1.z - p3.z);
		float l1 = ((p2.z - p3.z) * (x - p3.x) + (p3.x - p2.x) * (z - p3.z)) / det;
		float l2 = ((p3.z - p1.z) * (x - p3.x) + (p1.x - p3.x) * (z - p3.z)) / det;
		float l3 = 1.0f - l1 - l2;
		return l1 * p1.y + l2 * p2.y + l3 * p3.y;
	}

private:
	// Terrain
	unsigned int m_lengthX = 0;
	unsigned int m_lengthZ = 0;
	unsigned int m_terrainSize = 0;
	LODChunk*** m_chunks;

	// QuadTree
	QTRect m_quadArea{};
	QTRect m_searchArea{};
	StaticQuadTree<LODChunk*> m_quadtree;
	std::vector<LODChunk*> m_searchedChunks;

	// Threading
	std::mutex m_mutex;
	std::atomic<unsigned int> m_threadCount = 0;
	unsigned int m_maxThreads = 0;
	std::list<std::thread> m_threads;
};