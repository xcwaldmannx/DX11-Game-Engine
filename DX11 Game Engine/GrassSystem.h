#pragma once

#include <vector>

#include "ConstantBufferTypes.h"

#include "LODTerrain.h"

#include "VertexBuffer.h"

#include "Vec2f.h"
#include "Vec3f.h"
#include "Vertex.h"

struct GrassInstance {
	Vec3f offset;
	Vec3f color;
};

class GrassSystem {
public:
	GrassSystem(unsigned int numGrassBlades, LODTerrain* terrain);
	~GrassSystem();

	const InputLayoutPtr& getInputLayout();
	const VertexBufferPtr& getVertexBuffer();
	const InstanceBufferPtr& getInstanceBuffer();
	const IndexBufferPtr& getIndexBuffer();

	const TexturePtr& getTexture();

	const UINT getVertexCount();
	const UINT getIndexCount();

private:
	unsigned int numGrassBlades;
	LODTerrain* terrain;

	std::vector<Vertex> vertices;
	std::vector<GrassInstance> instances;

	MeshPtr mesh = nullptr;
	TexturePtr texture = nullptr;

	InputLayoutPtr inputLayout = nullptr;
	VertexBufferPtr vertexBuffer = nullptr;
	InstanceBufferPtr instanceBuffer = nullptr;
	IndexBufferPtr indexBuffer = nullptr;

};

