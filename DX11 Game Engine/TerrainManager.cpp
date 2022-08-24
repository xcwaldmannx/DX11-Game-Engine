#include "TerrainManager.h"

std::thread t;

TerrainManager::TerrainManager() : width(0), height(0), chunkCount(width * height), chunkLength(0) {
}

TerrainManager::TerrainManager(unsigned int width, unsigned int height, unsigned int chunkLength)
	: width(width), height(height), chunkCount(width * height), chunkLength(chunkLength) {

	for (unsigned int x = 0; x < width; x++) {
		for (unsigned int y = 0; y < height; y++) {
			// futures.push_back(std::async(std::launch::async, &TerrainManager::createChunk, this, x, y, chunkLength));
			chunks.push_back(createChunk(x, y, chunkLength));
		}
	}

	// t = std::thread{ &TerrainManager::updateChunks, this };
}

TerrainManager::~TerrainManager() {
}

Terrain* TerrainManager::createChunk(unsigned int x, unsigned int y, unsigned int chunkLength) {
	return new Terrain(x, y, chunkLength, Terrain::LOD_LEVEL_01);

}

void TerrainManager::updateChunks() {
	while (chunks.size() < chunkCount) {
		std::lock_guard<std::mutex> lock(chunksLock);
		for (int i = 0; i < futures.size(); i++) {
			if (isReady(futures[i])) {
				chunks.push_back(futures[i].get());
				futures.erase(futures.begin() + i);
			}
		}
	}

	futures.clear();
}

std::vector<Terrain*> TerrainManager::getChunks() {
	return chunks;
}

const unsigned int TerrainManager::getWidth() {
	return width;
}

const unsigned int TerrainManager::getHeight() {
	return height;
}

float TerrainManager::getHeightAt(float x, float z) {
	std::lock_guard<std::mutex> lock(chunksLock);
	unsigned int gridX = (unsigned int) max(0, floor(x / chunkLength));
	unsigned int gridY = (unsigned int) max(0, floor(z / chunkLength));
	if (chunks.size() > 0) {
		auto chunk = chunks[gridY * 2 + gridX];
		if (chunk) {
			return chunk->getHeightAt(max(0, x) - gridX * chunkLength, max(0, z) - gridY * chunkLength);
		}
		else {
			return 0;
		}
	}
	return 0;
}
