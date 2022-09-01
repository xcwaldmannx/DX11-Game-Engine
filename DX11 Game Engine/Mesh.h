#pragma once

#include <string>
#include <vector>

#include "Resource.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"

#include "VertexMesh.h"

#include "Vec3f.h"

struct MaterialSlot {
	size_t startIndex = 0;
	size_t numIndices = 0;
	size_t materialID = 0;
};

class Mesh : public Resource {
public:
	Mesh(const wchar_t* fullPath);
	~Mesh();

	const VertexBufferPtr& getVertexBuffer();
	const IndexBufferPtr& getIndexBuffer();

	const MaterialSlot getMaterialSlot(unsigned int slot);
	size_t materialSlotCount();

	std::vector<VertexMesh> getVertices();
	std::vector<unsigned int> getIndices();

public:
	const VertexBufferPtr& getVertexBufferBB();
	const IndexBufferPtr& getIndexBufferBB();

private:
	std::vector<VertexMesh> vertices;
	std::vector<unsigned int> indices;

	VertexBufferPtr vertexBuffer = nullptr;
	IndexBufferPtr indexBuffer = nullptr;

	std::vector<MaterialSlot> materialSlots{};

	// bounding box

	void logAxialMinAndMax(const Vec3f& vec);
	void generateAABB();
	Vec3f axialMin = {0, 0, 0};
	Vec3f axialMax = {0, 0, 0};
	VertexBufferPtr vertexBufferBB = nullptr;
	IndexBufferPtr indexBufferBB = nullptr;

	friend class DeviceContext;
};
