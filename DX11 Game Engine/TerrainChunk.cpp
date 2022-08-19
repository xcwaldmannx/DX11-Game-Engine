#include "TerrainChunk.h"

TerrainChunk::TerrainChunk() {
}

TerrainChunk::TerrainChunk(unsigned int gridX, unsigned int gridY, unsigned int length) {
	mesh = TerrainMesh::generateMesh(gridX, gridY, length, 64.0f, TerrainMesh::LOD_LEVEL_04);
}

TerrainChunk::~TerrainChunk() {
}
