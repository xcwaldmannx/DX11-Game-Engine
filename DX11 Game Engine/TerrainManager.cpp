#include "TerrainManager.h"

#include <thread>
#include <mutex>
#include <chrono>
#include <iostream>

#include "ThreadPool.h"

#define threaded 1

static std::mutex lock;

TerrainManager::TerrainManager() : width(0), height(0), chunkCount(width * height), chunkLength(0) {
}

TerrainManager::TerrainManager(unsigned int width, unsigned int height, unsigned int chunkLength)
	: width(width), height(height), chunkCount(width * height), chunkLength(chunkLength) {

	auto now = std::chrono::steady_clock::now();

	// create chunks
	for (unsigned int x = 0; x < width; x++) {
		for (unsigned int y = 0; y < height; y++) {
			if (threaded) {
				pool.pushTask([=] { TerrainManager::createChunk(chunks, x, y, chunkLength); });
			}
		}
	}

	long double time = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - now).count() / 1000.0;
	std::cout << time << std::endl;
}

TerrainManager::~TerrainManager() {
}

void TerrainManager::createChunk(std::vector<TerrainMesh>& chunks, unsigned int gridX, unsigned int gridY, unsigned int chunkLength) {
	lock.lock();
	chunks.push_back(TerrainMesh::generateMesh(gridX, gridY, chunkLength, 256.0f, TerrainMesh::LOD_LEVEL_01));
	lock.unlock();
}

const std::vector<TerrainMesh>& TerrainManager::getChunks() {
	return chunks;
}