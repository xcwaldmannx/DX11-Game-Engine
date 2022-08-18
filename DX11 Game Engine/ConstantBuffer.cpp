#include "ConstantBuffer.h"

#include <exception>

#include "RenderSystem.h"
#include "DeviceContext.h"

ConstantBuffer::ConstantBuffer(void* buffer, UINT sizeBuffer, RenderSystem* renderSystem) : renderSystem(renderSystem) {
	D3D11_BUFFER_DESC bufferDesc = {};
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.ByteWidth = sizeBuffer;
	bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bufferDesc.CPUAccessFlags = 0;
	bufferDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA initData = {};
	initData.pSysMem = buffer;

	HRESULT result = renderSystem->d3dDevice->CreateBuffer(&bufferDesc, &initData, &this->buffer);

	if (FAILED(result)) {
		throw std::exception("ConstantBuffer could not be created.");
	}

}

ConstantBuffer::~ConstantBuffer() {
	if (buffer) {
		buffer->Release();
	}
}

void ConstantBuffer::update(DeviceContextPtr context, void* buffer) {
	context->deviceContext->UpdateSubresource(this->buffer, NULL, NULL, buffer, NULL, NULL);
}
