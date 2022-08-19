#pragma once

#include <vector>
#include <future>

#include "ThreadPool.h"
#include "TerrainMesh.h"

class TerrainManager {
public:
	TerrainManager();
	TerrainManager(unsigned int width, unsigned int height, unsigned int chunkLength);
	~TerrainManager();

	static void createChunk(std::vector<TerrainMesh>& chunks, unsigned int gridX, unsigned int gridY, unsigned int chunkLength);
	static void pushBackFutures(int numMeshes, std::vector<std::future<TerrainMesh>>& meshes, std::vector<TerrainMesh>& chunks);
	const std::vector<TerrainMesh>& getChunks();

private:
	unsigned int width = 0;
	unsigned int height = 0;
	unsigned int chunkLength = 0;
	std::vector<TerrainMesh> chunks;
	std::vector<std::future<TerrainMesh>> meshes;
	std::thread worker;
	ThreadPool pool;
};

