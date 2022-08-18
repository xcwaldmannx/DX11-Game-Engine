#include "PixelShader.h"

#include <exception>

#include "RenderSystem.h"

PixelShader::PixelShader(const void* shaderByteCode, size_t sizeByteCode, RenderSystem* renderSystem) : renderSystem(renderSystem) {
	HRESULT result = renderSystem->d3dDevice->CreatePixelShader(shaderByteCode, sizeByteCode, nullptr, &pixelShader);

	if (FAILED(result)) {
		throw std::exception("PixelShader could not be created.");
	}
}

PixelShader::~PixelShader() {
	pixelShader->Release();
}
