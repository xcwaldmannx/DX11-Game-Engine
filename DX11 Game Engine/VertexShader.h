#pragma once

#include <d3d11.h>
#include "Prerequisites.h"

class VertexShader {
public:
	VertexShader(const void* shaderByteCode, size_t sizeByteCode, RenderSystem* renderSystem);
	~VertexShader();

private:
	bool init(const void* shaderByteCode, size_t sizeByteCode);

private:
	RenderSystem* renderSystem = nullptr;
	ID3D11VertexShader* vertexShader = nullptr;

	friend class RenderSystem;
	friend class DeviceContext;
};

