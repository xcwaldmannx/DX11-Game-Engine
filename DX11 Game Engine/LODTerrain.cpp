#include "LODTerrain.h"

#include <iostream>
#include <chrono>

#include "GraphicsEngine.h"
#include "PerlinNoise.h"

#define USE_THREADS 1

LODTerrain::LODTerrain(unsigned int lengthX, unsigned int lengthZ, unsigned int terrainSize) : m_lengthX(lengthX), m_lengthZ(lengthZ), m_terrainSize(terrainSize) {
	m_quadArea.position = { 0, 0 };
	m_quadArea.size = { (float) (m_lengthX * m_terrainSize), (float) (m_lengthZ * m_terrainSize) };

	m_quadtree.resize(m_quadArea);

	m_maxThreads = std::thread::hardware_concurrency();

	m_chunks = new LODChunk * *[m_lengthX];
	for (int i = 0; i < m_lengthX; i++) {
		m_chunks[i] = new LODChunk * [m_lengthZ];
	}

	initChunks();
}

LODTerrain::~LODTerrain() {

}

void LODTerrain::setViewArea(const QTRect& area) {
	m_searchArea = area;
}

void LODTerrain::initChunks() {
	for (int x = 0; x < m_lengthX; x++) {
		for (int z = 0; z < m_lengthZ; z++) {
			LODChunk* chunk = new LODChunk{};

			// absolute location
			chunk->area.position = { (float)(x * (m_terrainSize - 1)), (float)(z * (m_terrainSize - 1)) };
			chunk->area.size = { (float)m_terrainSize, (float)m_terrainSize };

			m_quadtree.insert(chunk, chunk->area);
			m_chunks[x][z] = chunk;
		}
	}
}

void LODTerrain::loadInitData(LODChunk* chunk) {
	std::lock_guard<std::mutex> lock(m_mutex);

	// allocate height memory
	chunk->terrainHeights = new float* [m_terrainSize];
	for (unsigned int j = 0; j < m_terrainSize; j++) {
		chunk->terrainHeights[j] = new float[m_terrainSize];
	}

	// seed and height data
	chunk->seed = PerlinNoise::generateSeed2D(m_terrainSize, 256.0f, true, nullptr);
	chunk->terrainHeights = PerlinNoise::generateNoise2D(m_terrainSize, chunk->seed, msb(m_terrainSize), nullptr);

	chunk->isProcessing = false;
	chunk->isLoadReady = true;

	m_threadCount.store(m_threadCount.load() - 1);
}

void LODTerrain::loadChunks() {
	if (m_threadCount.load() < m_maxThreads) {
		m_searchedChunks = m_quadtree.search(m_searchArea);

		for (int i = 0; i < m_searchedChunks.size(); i++) {
			if (m_searchedChunks[i]->isRenderReady) {
				continue;
			}

			if (!m_searchedChunks[i]->isLoadReady && !m_searchedChunks[i]->isProcessing) {
				m_searchedChunks[i]->isProcessing = true;
				m_threadCount.store(m_threadCount.load() + 1);
				m_threads.push_back(std::thread{ [=] { this->loadInitData(m_searchedChunks[i]); } });
			}

			if (m_searchedChunks[i]->isLoadReady && !m_searchedChunks[i]->isProcessing) {
				m_searchedChunks[i]->isProcessing = true;
				m_threadCount.store(m_threadCount.load() + 1);
				m_threads.push_front(std::thread{ [=] { this->generateChunk(m_searchedChunks[i]); } });
			}
		}
	}
}

void LODTerrain::generateChunk(LODChunk* chunk) {
	std::lock_guard<std::mutex> lock(m_mutex);

	chunk->vertices.reserve(chunk->area.size.x* chunk->area.size.y);
	for (int x = 0; x < m_terrainSize; x++) {
		for (int z = 0; z < m_terrainSize; z++) {

			// vertex
			float vx = x + chunk->area.position.x;
			float vy = chunk->terrainHeights[x][z];
			float vz = z + chunk->area.position.y;

			// texcoord
			float tx = (x % m_terrainSize);
			float ty = (z % m_terrainSize);

			// normal
			float nx = 0;
			float ny = 1;
			float nz = 0;

			if (x > 0 && x < m_terrainSize - 1 && z > 0 && z < m_terrainSize - 1) {
				float heightL = chunk->terrainHeights[x - 1][z];
				float heightR = chunk->terrainHeights[x + 1][z];
				float heightU = chunk->terrainHeights[x][z + 1];
				float heightD = chunk->terrainHeights[x][z - 1];

				nx = heightL - heightR;
				ny = 2.0f;
				nz = heightD - heightU;
			}

			LODVertex vertex = {
				vertex.position = Vec3f{ vx, vy, vz },
				vertex.texcoord = Vec2f{ tx, ty },
				vertex.normal = Vec3f{ nx, ny, nz }.normalize(),
			};
			chunk->vertices.push_back(vertex);
		}
	}

	// index buffer
	unsigned int numIndices = 6 * ((m_terrainSize - 1) * (m_terrainSize - 1));
	chunk->indices.reserve(numIndices);

	unsigned int currentIndex = 0;
	unsigned int step = 0;
	while (chunk->indices.size() < numIndices) {
		chunk->indices.push_back(currentIndex);
		chunk->indices.push_back(currentIndex + 1);
		chunk->indices.push_back(currentIndex + m_terrainSize + 1);

		chunk->indices.push_back(currentIndex + m_terrainSize + 1);
		chunk->indices.push_back(currentIndex + m_terrainSize);
		chunk->indices.push_back(currentIndex);
		currentIndex++;
		step++;
		if (step == m_terrainSize - 1) {
			step = 0;
			currentIndex++;
		}
	}

	void* shaderCode = nullptr;
	size_t sizeShaderCode = 0;

	GraphicsEngine::get()->getRenderSystem()->compileVertexShader(L"VertexMeshLayoutShader.hlsl", "main", &shaderCode, &sizeShaderCode);
	chunk->vertexBuffer = GraphicsEngine::get()->getRenderSystem()->createVertexBuffer(&chunk->vertices[0], sizeof(LODVertex), chunk->vertices.size(),
		VertexBuffer::INPUT_LAYOUT_STANDARD, shaderCode, sizeShaderCode);
	GraphicsEngine::get()->getRenderSystem()->releaseCompiledShader();
	chunk->indexBuffer = GraphicsEngine::get()->getRenderSystem()->createIndexBuffer(&chunk->indices[0], (UINT)chunk->indices.size());

	chunk->isProcessing = false;
	chunk->isRenderReady = true;

	m_threadCount.store(m_threadCount.load() - 1);
}

std::vector<LODChunk*> LODTerrain::getChunks(const QTRect& area) {
	std::lock_guard<std::mutex> lock(m_mutex);
	return m_quadtree.search(area);
}

void LODTerrain::draw(VertexShaderPtr vs, PixelShaderPtr ps, ConstantBufferPtr transform, ConstantBufferPtr lighting) {
	GraphicsEngine::get()->getRenderSystem()->setRasterizerState(RASTER_CULL_BACK);
	for (const auto& chunk : m_searchedChunks) {
		if (chunk->vertexBuffer && chunk->indexBuffer) {
			GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setVertexShader(vs);
			GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setPixelShader(ps);
			GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setVSConstantBuffer(0, transform);
			GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setVSConstantBuffer(1, lighting);
			GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setPSConstantBuffer(0, transform);
			GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setPSConstantBuffer(1, lighting);
			GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setVertexBuffer(chunk->vertexBuffer);
			GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setIndexBuffer(chunk->indexBuffer);
			GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->drawIndexedTriangleList(chunk->indices.size(), 0, 0);
		}
	}
}

float LODTerrain::getHeightAt(float x, float z) {
	std::lock_guard<std::mutex> lock(m_mutex);
	// 0______1
	// |      |
	// |      |
	// |______|
	// 2      3

	if (!m_chunks) {
		return 0;
	}

	unsigned int gridX = floor(max(0, x / (m_terrainSize - 1)));
	unsigned int gridZ = floor(max(0, z / (m_terrainSize - 1)));

	if (gridX >= m_lengthX || gridZ >= m_lengthZ) {
		return 0;
	}

	x = max(0, x) - gridX * (m_terrainSize - 1);
	z = max(0, z) - gridZ * (m_terrainSize - 1);

	unsigned int heightX = (unsigned int) x;
	unsigned int heightZ = (unsigned int) z;

	const LODChunk* chunk = m_chunks[gridX][gridZ];

	if (!chunk || !chunk->isRenderReady) {
		return 0;
	}

	// location on face of terrain, between 0 and 1
	float faceX = -1;
	float faceZ = -1;
	float whole;

	faceX = std::modf(x, &whole);
	faceZ = std::modf(z, &whole);

	if (faceX <= 1 - faceZ) {
		if (gridX <= m_terrainSize - 2 && gridZ <= m_terrainSize - 2) {
			Vec3f v0 = Vec3f(0, chunk->terrainHeights[heightX]    [heightZ], 0);
			Vec3f v1 = Vec3f(1, chunk->terrainHeights[heightX + 1][heightZ], 0);
			Vec3f v2 = Vec3f(0, chunk->terrainHeights[heightX]    [heightZ + 1], 1);
			return barryCentric(v0, v1, v2, faceX, faceZ);
		}
		else if (gridX >= 1 && gridZ >= 1) {
			Vec3f v0 = Vec3f(0, chunk->terrainHeights[heightX]    [heightZ], 0);
			Vec3f v1 = Vec3f(1, chunk->terrainHeights[heightX - 1][heightZ], 0);
			Vec3f v2 = Vec3f(0, chunk->terrainHeights[heightX]    [heightZ - 1], 1);
			return barryCentric(v0, v1, v2, faceX, faceZ);
		}
	}
	else {
		if (gridX <= m_terrainSize - 2 && gridZ <= m_terrainSize - 2) {
			Vec3f v1 = Vec3f(1, chunk->terrainHeights[heightX + 1][heightZ], 0);
			Vec3f v2 = Vec3f(1, chunk->terrainHeights[heightX + 1][heightZ + 1], 1);
			Vec3f v3 = Vec3f(0, chunk->terrainHeights[heightX]    [heightZ + 1], 1);
			return barryCentric(v1, v2, v3, faceX, faceZ);
		}
		else if (gridX >= 1 && gridZ >= 1) {
			Vec3f v1 = Vec3f(1, chunk->terrainHeights[heightX - 1][heightZ], 0);
			Vec3f v2 = Vec3f(1, chunk->terrainHeights[heightX - 1][heightZ - 1], 1);
			Vec3f v3 = Vec3f(0, chunk->terrainHeights[heightX]    [heightZ - 1], 1);
			return barryCentric(v1, v2, v3, faceX, faceZ);
		}
	}
	return 0;
}
