#pragma once

#include <d3d11.h>
#include "Prerequisites.h"

class VertexBuffer {
public:
	VertexBuffer(void* listVertices, UINT sizeVertex, UINT sizeList, void* shaderByteCode, UINT sizeByteShaderCode, RenderSystem* renderSystem);
	~VertexBuffer();

	UINT getSizeVertexList();

private:
	RenderSystem* renderSystem = nullptr;

	UINT sizeVertex = 0;
	UINT sizeList = 0;

	ID3D11Buffer* buffer = nullptr;
	ID3D11InputLayout* layout = nullptr;

	friend class DeviceContext;
};

