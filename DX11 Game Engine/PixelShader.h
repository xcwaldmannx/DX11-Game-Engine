#pragma once

#include <d3d11.h>
#include "Prerequisites.h"

class PixelShader {
public:
	PixelShader(const void* shaderByteCode, size_t sizeByteCode, RenderSystem* renderSystem);
	~PixelShader();

private:
	RenderSystem* renderSystem = nullptr;

	ID3D11PixelShader* pixelShader = nullptr;

	friend class RenderSystem;
	friend class DeviceContext;
};

