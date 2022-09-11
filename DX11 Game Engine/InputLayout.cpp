#include "InputLayout.h"

#define WINDOWS_IGNORE_PACKING_MISMATCH
#include <windows.h>

InputLayout::InputLayout(const std::vector<D3D11_INPUT_ELEMENT_DESC>& elements, const wchar_t* shaderFilename, const char* shaderEntrypoint, RenderSystem* renderSystem) {
	void* shaderByteCode = nullptr;
	size_t sizeShaderByteCode = 0;
	renderSystem->compileVertexShader(shaderFilename, shaderEntrypoint, &shaderByteCode, &sizeShaderByteCode);
	HRESULT result = renderSystem->d3dDevice->CreateInputLayout(&elements[0], elements.size(), shaderByteCode, sizeShaderByteCode, &layout);
	renderSystem->releaseCompiledShader();

	if (FAILED(result)) {
		throw std::exception("InputLayout could not be created.");
	}
}

InputLayout::~InputLayout() {
	layout->Release();
}
