#pragma once

#include <d3d11.h>
#include "Prerequisites.h"

class IndexBuffer {
public:
	IndexBuffer(void* listIndices, UINT sizeList, RenderSystem* renderSystem);
	~IndexBuffer();

	UINT getSizeIndexList();

private:
	RenderSystem* renderSystem = nullptr;

	UINT sizeList = 0;

	ID3D11Buffer* buffer = nullptr;

	friend class DeviceContext;
};

