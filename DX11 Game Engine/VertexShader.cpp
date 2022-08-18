#include "VertexShader.h"

#include <exception>

#include "RenderSystem.h"

VertexShader::VertexShader(const void* shaderByteCode, size_t sizeByteCode, RenderSystem* renderSystem) : renderSystem(renderSystem) {
	HRESULT result = renderSystem->d3dDevice->CreateVertexShader(shaderByteCode, sizeByteCode, nullptr, &vertexShader);

	if (FAILED(result)) {
		throw std::exception("VertexShader could not be created.");
	}
}

VertexShader::~VertexShader() {
	vertexShader->Release();
}
