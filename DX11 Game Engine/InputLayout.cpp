#include "InputLayout.h"

#define WINDOWS_IGNORE_PACKING_MISMATCH
#include <windows.h>

InputLayout::InputLayout(const std::vector<InputElement>& elements, const wchar_t* shaderFilename, const char* shaderEntrypoint, RenderSystem* renderSystem) {
	std::vector<D3D11_INPUT_ELEMENT_DESC> desc;
	desc.resize(elements.size());
	for (int i = 0; i < desc.size(); i++) {
		desc[i].SemanticName = elements[i].semanticName;
		desc[i].SemanticIndex = elements[i].semanticIndex;
		desc[i].Format = elements[i].format;
		desc[i].InputSlot = elements[i].inputSlot;
		desc[i].AlignedByteOffset = elements[i].alignedByteOffset;
		desc[i].InputSlotClass = elements[i].inputSlotClass;
		desc[i].InstanceDataStepRate = elements[i].instanceDataStepRate;
	}

	void* shaderByteCode = nullptr;
	size_t sizeShaderByteCode = 0;
	renderSystem->compileVertexShader(shaderFilename, shaderEntrypoint, &shaderByteCode, &sizeShaderByteCode);
	HRESULT result = renderSystem->d3dDevice->CreateInputLayout(&desc[0], desc.size(), shaderByteCode, sizeShaderByteCode, &layout);
	renderSystem->releaseCompiledShader();

	if (FAILED(result)) {
		throw std::exception("InputLayout could not be created.");
	}
}

InputLayout::~InputLayout() {
	layout->Release();
}
