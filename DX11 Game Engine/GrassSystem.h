#pragma once

#include <vector>

#include "ConstantBufferTypes.h"

#include "VertexBuffer.h"

#include "LODTerrain.h"

#include "Vec2f.h"
#include "Vec3f.h"

struct GrassVertex {
	Vec3f position;
	Vec2f texcoord;
	Vec3f normal;
};

struct GrassInstance {
	Vec3f offset;
};

class GrassSystem {
public:
	GrassSystem(unsigned int numGrassBlades, LODTerrain* terrain);
	~GrassSystem();

	const VertexBufferPtr& getVertexBuffer();
	const InstanceBufferPtr& getInstanceBuffer();
	const IndexBufferPtr& getIndexBuffer();

	const TexturePtr& getTexture();

	const UINT getVertexCount();
	const UINT getIndexCount();

private:
	unsigned int numGrassBlades;
	LODTerrain* terrain;

	std::vector<GrassVertex> vertices;
	std::vector<GrassInstance> instances;

	MeshPtr mesh = nullptr;
	TexturePtr texture = nullptr;

	VertexBufferPtr vertexBuffer = nullptr;
	InstanceBufferPtr instanceBuffer = nullptr;
	IndexBufferPtr indexBuffer = nullptr;

};

