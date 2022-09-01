#pragma once

#include <d3d11.h>

#include <vector>

#include "Prerequisites.h"

struct InstanceData {
};

class InstanceBuffer {
public:
	InstanceBuffer(void* instances, UINT dataSize, UINT numInstances, RenderSystem* renderSystem);
	~InstanceBuffer();

private:
	RenderSystem* renderSystem = nullptr;
	ID3D11Buffer* buffer = nullptr;

	UINT dataSize = 0;

	friend class DeviceContext;
};

