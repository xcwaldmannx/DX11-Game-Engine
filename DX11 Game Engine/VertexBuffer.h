#pragma once

#include <d3d11.h>

#include "Prerequisites.h"

class VertexBuffer {
public:
	VertexBuffer(void* listVertices, UINT sizeVertex, UINT sizeList, RenderSystem* renderSystem);
	~VertexBuffer();

	UINT getSizeVertexList();

private:
	D3D11_INPUT_ELEMENT_DESC layoutStandard[3] = {
		// semantic name - semantic index - format - input slot - aligned byte offset - input slot class - instance data step rate
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,    0,  0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,       0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT,    0, 20, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	D3D11_INPUT_ELEMENT_DESC layoutStandardInst[4] = {
		// semantic name - semantic index - format - input slot - aligned byte offset - input slot class - instance data step rate
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,    0,  0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,       0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT,    0, 20, D3D11_INPUT_PER_VERTEX_DATA, 0 },

		{ "TEXCOORD", 3, DXGI_FORMAT_R32G32B32_FLOAT,    1,  0, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
	};

private:
	RenderSystem* renderSystem = nullptr;

	UINT sizeVertex = 0;
	UINT sizeList = 0;

	ID3D11Buffer* buffer = nullptr;

	friend class DeviceContext;
};

