#include "InstanceBuffer.h"

#include "RenderSystem.h"

InstanceBuffer::InstanceBuffer(void* instances, UINT dataSize, UINT numInstances, void* shaderByteCode, UINT sizeByteShaderCode, RenderSystem* renderSystem)
	: renderSystem(renderSystem), buffer(0) {
	D3D11_BUFFER_DESC bufferDesc = {};
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.ByteWidth = dataSize * numInstances;
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.CPUAccessFlags = 0;
	bufferDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA initData = {};
	initData.pSysMem = instances;

	this->dataSize = dataSize;

	HRESULT result = renderSystem->d3dDevice->CreateBuffer(&bufferDesc, &initData, &buffer);

	if (FAILED(result)) {
		throw std::exception("VertexBuffer could not be created.");
	}
}

InstanceBuffer::~InstanceBuffer() {
	buffer->Release();
}
