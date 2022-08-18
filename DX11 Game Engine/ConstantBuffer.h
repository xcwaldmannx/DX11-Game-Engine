#pragma once

#include <d3d11.h>
#include "Prerequisites.h"

class ConstantBuffer {
public:
	ConstantBuffer(void* buffer, UINT sizeBuffer, RenderSystem* renderSystem);
	~ConstantBuffer();

	void update(DeviceContextPtr context, void* buffer);

private:
	RenderSystem* renderSystem = nullptr;

	ID3D11Buffer* buffer = nullptr;

	friend class DeviceContext;
};

