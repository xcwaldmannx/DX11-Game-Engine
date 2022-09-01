#pragma once

#include <d3d11.h>
#include <vector>

#include "RenderSystem.h"

struct InputElement {
	const char* semanticName = "";
	unsigned int semanticIndex = 0;
	DXGI_FORMAT format = DXGI_FORMAT_R32G32B32_FLOAT;
	unsigned int inputSlot = 0;
	unsigned int alignedByteOffset = 0;
	D3D11_INPUT_CLASSIFICATION inputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	unsigned int instanceDataStepRate = 0;
};

class InputLayout {
public:
	InputLayout(const std::vector<InputElement>& elements, const wchar_t* shaderFilename, const char* shaderEntrypoint, RenderSystem* renderSystem);
	~InputLayout();

private:
	ID3D11InputLayout* layout = nullptr;

	friend class DeviceContext;
};