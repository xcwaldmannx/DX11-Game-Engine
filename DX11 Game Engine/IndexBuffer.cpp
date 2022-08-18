#include "IndexBuffer.h"

#include <exception>

#include "RenderSystem.h"

IndexBuffer::IndexBuffer(void* listIndices, UINT sizeList, RenderSystem* renderSystem) : renderSystem(renderSystem), buffer(0) {
	D3D11_BUFFER_DESC bufferDesc = {};
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.ByteWidth = 4 * sizeList;
	bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bufferDesc.CPUAccessFlags = 0;
	bufferDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA initData = {};
	initData.pSysMem = listIndices;
	this->sizeList = sizeList;

	HRESULT result = renderSystem->d3dDevice->CreateBuffer(&bufferDesc, &initData, &buffer);

	if (FAILED(result)) {
		throw std::exception("IndexBuffer could not be created.");
	}
}

IndexBuffer::~IndexBuffer() {
	buffer->Release();
}

UINT IndexBuffer::getSizeIndexList() {
	return sizeList;
}
