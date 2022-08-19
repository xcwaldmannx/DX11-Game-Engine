#include "TerrainManager.h"

#include <thread>
#include <mutex>
#include <chrono>
#include <iostream>

#include "ThreadPool.h"

#define threaded 1

static std::mutex lock;

TerrainManager::TerrainManager() : width(0), height(0), chunkLength(0) {
}

TerrainManager::TerrainManager(unsigned int width, unsigned int height, unsigned int chunkLength) : width(width), height(height), chunkLength(chunkLength) {

	auto now = std::chrono::steady_clock::now();

	// create chunks
	for (unsigned int x = 0; x < width; x++) {
		for (unsigned int y = 0; y < height; y++) {
			if (threaded) {
				pool.pushTask([=] { TerrainManager::createChunk(chunks, x, y, chunkLength); });
				//meshes.push_back(std::async(std::launch::async, &TerrainMesh::generateMesh, x, y, chunkLength, 128.0f, TerrainMesh::LOD_LEVEL_02));
			}
		}
	}

	if (threaded) {
		//worker = std::thread{ this->pushBackFutures, width * height, std::ref(meshes), std::ref(chunks) };
	}

	long double time = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - now).count() / 1000.0;
	std::cout << time << std::endl;
}

TerrainManager::~TerrainManager() {
}

void TerrainManager::createChunk(std::vector<TerrainMesh>& chunks, unsigned int gridX, unsigned int gridY, unsigned int chunkLength) {
	chunks.push_back(TerrainMesh::generateMesh(gridX, gridY, chunkLength, 128.0f, TerrainMesh::LOD_LEVEL_02));
}

void TerrainManager::pushBackFutures(int numMeshes, std::vector<std::future<TerrainMesh>>& meshes, std::vector<TerrainMesh>& chunks) {
	while (chunks.size() < numMeshes) {
		if (meshes.size() == numMeshes) {
			for (int i = 0; i < numMeshes; i++) {
				lock.lock();
				chunks.push_back(meshes[i].get());
				lock.unlock();
			}
			break;
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}
}

const std::vector<TerrainMesh>& TerrainManager::getChunks() {
	return chunks;
}