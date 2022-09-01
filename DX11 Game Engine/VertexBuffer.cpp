#include "VertexBuffer.h"

#include <exception>

#include "RenderSystem.h"

VertexBuffer::VertexBuffer(void* listVertices, UINT sizeVertex, UINT sizeList, RenderSystem* renderSystem) : renderSystem(renderSystem), buffer(0) {
	D3D11_BUFFER_DESC bufferDesc = {};
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.ByteWidth = sizeVertex * sizeList;
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.CPUAccessFlags = 0;
	bufferDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA initData = {};
	initData.pSysMem = listVertices;
	this->sizeVertex = sizeVertex;
	this->sizeList = sizeList;

	HRESULT result = renderSystem->d3dDevice->CreateBuffer(&bufferDesc, &initData, &buffer);

	if (FAILED(result)) {
		throw std::exception("VertexBuffer could not be created.");
	}
}

VertexBuffer::~VertexBuffer() {
	buffer->Release();
}

UINT VertexBuffer::getSizeVertexList() {
	return sizeList;
}
