#pragma once

#include <vector>

#include "VertexShader.h"
#include "PixelShader.h"
#include "ConstantBuffer.h"
#include "RenderSystem.h"

class GraphicsEngine;

class Material {
public:
	Material(const wchar_t* vertexShaderPath, const wchar_t* pixelShaderPath, unsigned int textureCount);
	~Material();

	void addTexture(const TexturePtr& texture);
	void removeTexture(unsigned int index);

	void setData(void* data, unsigned int size);

	void setCullMode(RASTER_MODE mode);
	RASTER_MODE getCullMode();

private:
	VertexShaderPtr vertexShader = nullptr;
	PixelShaderPtr pixelShader = nullptr;

	ConstantBufferPtr constantBuffer = nullptr;

	unsigned int textureCount = 1;
	TextureArrayPtr textures = nullptr;

	RASTER_MODE cullMode = RASTER_CULL_BACK;

	friend class GraphicsEngine;
};

