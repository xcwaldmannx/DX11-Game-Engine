#pragma once

#include <vector>
#include <future>
#include <mutex>

#include "Terrain.h"

class TerrainManager {
public:
	TerrainManager();
	TerrainManager(unsigned int width, unsigned int height, unsigned int chunkLength);
	~TerrainManager();

	Terrain* createChunk(unsigned int x, unsigned int y, unsigned int chunkLength);
	void updateChunks();
	std::vector<Terrain*> getChunks();
	const unsigned int getWidth();
	const unsigned int getHeight();

	float getHeightAt(float x, float z);

	bool isReady(std::future<Terrain*> const& f) {
		return f.wait_for(std::chrono::seconds(0)) == std::future_status::ready;
	}

private:
	unsigned int width = 0;
	unsigned int height = 0;
	unsigned int chunkCount = 0;
	unsigned int chunkLength = 0;

	std::mutex chunksLock;
	std::mutex futuresLock;
	std::mutex heightLock;

	std::vector<Terrain*> chunks;
	std::vector<std::future<Terrain*>> futures;
};

