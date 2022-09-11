#include "LODTerrain.h"

#include <iostream>
#include <chrono>

#include "GraphicsEngine.h"
#include "PerlinNoise.h"

LODTerrain::LODTerrain(unsigned int lengthX, unsigned int lengthZ, unsigned int terrainSize) : m_lengthX(lengthX), m_lengthZ(lengthZ), m_terrainSize(terrainSize) {
	m_quadArea.position = { 0, 0 };
	m_quadArea.size = { (float) (m_lengthX * m_terrainSize), (float) (m_lengthZ * m_terrainSize) };

	m_quadtree.resize(m_quadArea);

	m_maxThreads = std::thread::hardware_concurrency();

	m_chunks = new LODChunk * *[m_lengthX];
	for (int i = 0; i < m_lengthX; i++) {
		m_chunks[i] = new LODChunk * [m_lengthZ];
	}

	m_diffInChunkVectors.reserve(m_lengthX * m_lengthZ);

	initChunks();
}

LODTerrain::~LODTerrain() {

}

void LODTerrain::setCameraToTrack(Camera* camera, unsigned int searchAreaSize) {
	m_camera = camera;
	m_searchAreaSize = searchAreaSize;
	lodSystem = new LODSystem{ m_terrainSize, searchAreaSize, 7 };
}

void LODTerrain::initChunks() {
	for (int x = 0; x < m_lengthX; x++) {
		for (int z = 0; z < m_lengthZ; z++) {
			LODChunk* chunk = new LODChunk();

			// absolute location
			float locX = (float)(x * (m_terrainSize - 1));
			float locZ = (float)(z * (m_terrainSize - 1));

			chunk->area.position = { locX, locZ };
			chunk->area.size = { (float) m_terrainSize, (float) m_terrainSize };

			m_quadtree.insert(chunk, chunk->area);
			m_chunks[x][z] = chunk;
			m_allChunks.push_back(chunk);
		}
	}
}

void LODTerrain::loadInitData(LODChunk* chunk) {
	std::lock_guard<std::mutex> lock(m_mutex);

	// set LODRadius
	float locX = chunk->area.position.x + chunk->area.size.x / 2.0f;
	float locZ = chunk->area.position.y + chunk->area.size.y / 2.0f;
	chunk->radius = {
		{locX, locZ},
		{lodSystem->sizePerLevel()},
		{lodSystem->maxLODLevel()},
	};

	// seed and height data
	chunk->seed = PerlinNoise::generateSeed2D(m_terrainSize, 256.0f, true, nullptr);
	chunk->terrainHeights = PerlinNoise::generateNoise2D(m_terrainSize, chunk->seed, msb(m_terrainSize), nullptr);

	chunk->isProcessing = false;
	chunk->isLoadReady = true;

	m_threadCount--;
}

void LODTerrain::loadChunks() {

	// update the search area for quadtree
	Point2f position = {
		m_camera->getWorld().getTranslation().x - (float) m_searchAreaSize / 2.0f,
		m_camera->getWorld().getTranslation().z - (float) m_searchAreaSize / 2.0f,
	};

	Point2f size = {
		(float) m_searchAreaSize,
		(float) m_searchAreaSize,
	};

	m_searchArea = QTRect(position, size);
	m_searchedChunks = getChunks(m_searchArea);

	// update LODSystem position
	lodSystem->updatePosition(m_camera->getWorld().getTranslation().x, m_camera->getWorld().getTranslation().z);

	for (int i = 0; i < m_searchedChunks.size(); i++) {
		if (m_searchedChunks[i]->isRenderReady) {
			float lod = lodSystem->getLODOf(m_searchedChunks[i]->radius);

			/*
			if (lod >= lodSystem->maxLODLevel()) {
				unloadChunk(m_searchedChunks[i]);
				continue;
			}
			*/

			// chunk is already loaded at correct LOD
			if (lod == m_searchedChunks[i]->radius.lod) {
				continue;
			}
			// chunk is not at correct LOD, must be reloaded
			else {
				if (m_threadCount < m_maxThreads) {
					m_searchedChunks[i]->radius.lod = lod;
					m_searchedChunks[i]->isProcessing = true;
					m_threadCount++;
					m_threads.push_front(std::thread{ [=] { this->generateChunkAtLOD(m_searchedChunks[i], lodLevels[m_searchedChunks[i]->radius.lod]); } });
					continue;
				}
			}
		}

		// init data load
		if (m_threadCount < m_maxThreads) {
			if (!m_searchedChunks[i]->isLoadReady && !m_searchedChunks[i]->isProcessing) {
				m_searchedChunks[i]->isProcessing = true;
				m_threadCount++;
				m_threads.push_back(std::thread{ [=] { this->loadInitData(m_searchedChunks[i]); } });
				continue;
			}
		}

		// init render load
		if (!m_searchedChunks[i]->isRenderReady && m_searchedChunks[i]->isLoadReady && !m_searchedChunks[i]->isProcessing) {
			m_searchedChunks[i]->isProcessing = true;
			m_threadCount++;
			m_threads.push_front(std::thread{ [=] { this->generateChunkAtLOD(m_searchedChunks[i], lodLevels[m_searchedChunks[i]->radius.lod]); } });
			continue;
		}
	}
}

void LODTerrain::unloadChunk(LODChunk* chunk) {
	chunk->vertices.clear();
	chunk->vertices.shrink_to_fit();
	chunk->indices.clear();
	chunk->indices.shrink_to_fit();

	if (chunk->vertexBuffer) {
		chunk->vertexBuffer->~VertexBuffer();
		chunk->vertexBuffer = nullptr;
	}

	if (chunk->indexBuffer) {
		chunk->indexBuffer->~IndexBuffer();
		chunk->indexBuffer = nullptr;
	}

	chunk->isRenderReady = false;
	chunk->isProcessing = false;
}

void LODTerrain::generateChunk(LODChunk* chunk) {
	std::lock_guard<std::mutex> lock(m_mutex);

	chunk->vertices.clear();
	chunk->vertices.reserve(chunk->area.size.x * chunk->area.size.y);
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

			Vertex vertex = {
				vertex.position = Vec3f{ vx, vy, vz },
				vertex.texcoord = Vec2f{ tx, ty },
				vertex.normal = Vec3f{ nx, ny, nz }.normalize(),
			};
			chunk->vertices.push_back(vertex);
		}
	}

	// index buffer
	unsigned int numIndices = 6 * ((m_terrainSize - 1) * (m_terrainSize - 1));
	chunk->indices.clear();
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

	chunk->vertexBuffer = GraphicsEngine::get()->getRenderSystem()->createVertexBuffer(&chunk->vertices[0], sizeof(Vertex), chunk->vertices.size());
	// input layout creation ->
	chunk->indexBuffer = GraphicsEngine::get()->getRenderSystem()->createIndexBuffer(&chunk->indices[0], (UINT)chunk->indices.size());

	chunk->isProcessing = false;
	chunk->isRenderReady = true;

	m_threadCount--;
}

void LODTerrain::generateChunkAtLOD(LODChunk* chunk, const unsigned int lod) {
	std::lock_guard<std::mutex> lock(m_mutex);

	float lodXf = 0;
	float lodZf = 0;
	unsigned int lodXi = 0;
	unsigned int lodZi = 0;
	const float lodIncrement = lod;
	unsigned int lodTerrainSize = 1 + m_terrainSize / (float) lod;

	chunk->vertices.clear();
	chunk->vertices.shrink_to_fit();
	chunk->vertices.reserve(lodTerrainSize * lodTerrainSize);

	for (int x = 0; x < lodTerrainSize; x++) {
		lodZf = 0;
		lodZi = 0;
		for (int z = 0; z < lodTerrainSize; z++) {

			// vertex
			float vx = lodXf + chunk->area.position.x;
			if (lodXi >= m_terrainSize) lodXi = m_terrainSize - 1;
			if (lodZi >= m_terrainSize) lodZi = m_terrainSize - 1;
			float vy = chunk->terrainHeights[lodXi][lodZi];
			float vz = lodZf + chunk->area.position.y;

			// texcoord
			float tx = (lodXi % m_terrainSize);
			float ty = (lodZi % m_terrainSize);

			// normal
			float nx = 0;
			float ny = 1;
			float nz = 0;

			if (lodXi > 0 && lodXi < m_terrainSize - 1 && lodZi > 0 && lodZi < m_terrainSize - 1) {
				float heightL = chunk->terrainHeights[lodXi - 1][lodZi];
				float heightR = chunk->terrainHeights[lodXi + 1][lodZi];
				float heightU = chunk->terrainHeights[lodXi][lodZi + 1];
				float heightD = chunk->terrainHeights[lodXi][lodZi - 1];

				nx = heightL - heightR;
				ny = 2.0f;
				nz = heightD - heightU;
			}

			Vertex vertex = {
				vertex.position = Vec3f{ vx, vy, vz },
				vertex.texcoord = Vec2f{ tx, ty },
				vertex.normal = Vec3f{ nx, ny, nz }.normalize(),
			};
			chunk->vertices.push_back(vertex);

			lodZf += lodIncrement;
			lodZi += lodIncrement;
		}
		lodXf += lodIncrement;
		lodXi += lodIncrement;
	}

	// index buffer
	unsigned int numIndices = 6 * ((lodTerrainSize - 1) * (lodTerrainSize - 1));
	chunk->indices.clear();
	chunk->indices.shrink_to_fit();
	chunk->indices.reserve(numIndices);

	unsigned int currentIndex = 0;
	unsigned int step = 0;
	while (chunk->indices.size() < numIndices) {
		chunk->indices.push_back(currentIndex);
		chunk->indices.push_back(currentIndex + 1);
		chunk->indices.push_back(currentIndex + lodTerrainSize + 1);

		chunk->indices.push_back(currentIndex + lodTerrainSize + 1);
		chunk->indices.push_back(currentIndex + lodTerrainSize);
		chunk->indices.push_back(currentIndex);
		currentIndex++;
		step++;
		if (step == lodTerrainSize - 1) {
			step = 0;
			currentIndex++;
		}
	}

	void* shaderCode = nullptr;
	size_t sizeShaderCode = 0;

	chunk->vertexBuffer = GraphicsEngine::get()->getRenderSystem()->createVertexBuffer(&chunk->vertices[0], sizeof(Vertex), chunk->vertices.size());
	// input layout creation ->
	chunk->indexBuffer = GraphicsEngine::get()->getRenderSystem()->createIndexBuffer(&chunk->indices[0], (UINT)chunk->indices.size());

	chunk->isProcessing = false;
	chunk->isRenderReady = true;

	m_threadCount--;
}

std::vector<LODChunk*> LODTerrain::getChunks(const QTRect& area) {
	std::lock_guard<std::mutex> lock(m_mutex);
	return m_quadtree.search(area);
}

void LODTerrain::draw(VertexShaderPtr vs, PixelShaderPtr ps, ConstantBufferPtr transform, ConstantBufferPtr lighting) {
	GraphicsEngine::get()->getRenderSystem()->setRasterizerState(RASTER_CULL_BACK);
	for (const auto& chunk : m_searchedChunks) {
		if (chunk->isRenderReady && chunk->vertexBuffer && chunk->indexBuffer) {
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
