#pragma once

#include <d3d11.h>
#include "Prerequisites.h"

class DeviceContext {
public:
	DeviceContext(ID3D11DeviceContext* deviceContext, RenderSystem* renderSystem);
	~DeviceContext();

	void setViewportSize(UINT width, UINT height);

	void clearRenderTargetColor(const SwapChainPtr& swapchain, float r, float g, float b, float a);
	void drawTriangleList(UINT vertexCount, UINT startVertexIndex);
	void drawIndexedTriangleList(UINT indexCount, UINT startVertexIndex, UINT startIndexIndex);
	void drawTriangleStrip(UINT vertexCount, UINT startVertexIndex);
	void drawWireframe(UINT indexCount, UINT startVertexIndex, UINT startIndexIndex);

	void setVertexBuffer(const VertexBufferPtr& vertexBuffer);
	void setIndexBuffer(const IndexBufferPtr& indexBuffer);

	void setVSConstantBuffer(UINT slot, const ConstantBufferPtr& constantBuffer);
	void setPSConstantBuffer(UINT slot, const ConstantBufferPtr& constantBuffer);

	void setVertexShader(const VertexShaderPtr& vertexShader);
	void setPixelShader(const PixelShaderPtr& pixelShader);

	void setVSTexture(UINT slot, const TexturePtr& texture);
	void setPSTexture(UINT slot, const TexturePtr& texture);

	void setVSTextureArray(UINT slot, const TextureArrayPtr& texture, UINT textureCount);
	void setPSTextureArray(UINT slot, const TextureArrayPtr& texture, UINT textureCount);

private:
	RenderSystem* renderSystem = nullptr;

	ID3D11DeviceContext* deviceContext = nullptr;

	friend class ConstantBuffer;
};

