#pragma once

#include "TerrainMesh.h"

class TerrainChunk {
public:
	TerrainChunk();
	TerrainChunk(unsigned int gridX, unsigned int gridY, unsigned int length);
	~TerrainChunk();

	TerrainMesh mesh;

};

