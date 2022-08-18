#include "VertexBuffer.h"

#include <exception>

#include "RenderSystem.h"

VertexBuffer::VertexBuffer(void* listVertices, UINT sizeVertex, UINT sizeList, void* shaderByteCode, UINT sizeByteShaderCode,
	RenderSystem* renderSystem) : renderSystem(renderSystem), buffer(0), layout(0) {
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

	D3D11_INPUT_ELEMENT_DESC layout[] = {
		// semantic name - semantic index - format - input slot - aligned byte offset - input slot class - instance data step rate
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,    0,  0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,       0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT,    0, 20, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	UINT sizeLayout = ARRAYSIZE(layout);

	result = renderSystem->d3dDevice->CreateInputLayout(layout, sizeLayout, shaderByteCode, sizeByteShaderCode, &this->layout);

	if (FAILED(result)) {
		throw std::exception("InputLayout could not be created.");
	}
}

VertexBuffer::~VertexBuffer() {
	layout->Release();
	buffer->Release();
}

UINT VertexBuffer::getSizeVertexList() {
	return sizeList;
}
