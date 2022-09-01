#include "DeviceContext.h"

#include <exception>

#include "SwapChain.h"
#include "VertexBuffer.h"
#include "InstanceBuffer.h"
#include "IndexBuffer.h"
#include "ConstantBuffer.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "Texture.h"
#include "TextureArray.h"

DeviceContext::DeviceContext(ID3D11DeviceContext* deviceContext, RenderSystem* renderSystem) : deviceContext(deviceContext), renderSystem(renderSystem) {
}

DeviceContext::~DeviceContext() {
	deviceContext->Release();
}

void DeviceContext::setViewportSize(UINT width, UINT height) {
	D3D11_VIEWPORT viewport = {};
	viewport.Width = (float) width;
	viewport.Height = (float) height;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	deviceContext->RSSetViewports(1, &viewport);
}

void DeviceContext::clearRenderTargetColor(const SwapChainPtr& swapchain, float r, float g, float b, float a) {
	FLOAT clearColor[] = {r, g, b, a};
	deviceContext->ClearRenderTargetView(swapchain->renderTargetView, clearColor);
	deviceContext->ClearDepthStencilView(swapchain->depthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1, 0);
	deviceContext->OMSetRenderTargets(1, &swapchain->renderTargetView, swapchain->depthStencilView);
}

void DeviceContext::drawTriangleList(UINT vertexCount, UINT startVertexIndex) {
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	deviceContext->Draw(vertexCount, startVertexIndex);
}

void DeviceContext::drawIndexedTriangleList(UINT indexCount, UINT startVertexIndex, UINT startIndexIndex) {
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	deviceContext->DrawIndexed(indexCount, startIndexIndex, startVertexIndex);
}

void DeviceContext::drawTriangleStrip(UINT vertexCount, UINT startVertexIndex) {
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	deviceContext->Draw(vertexCount, startVertexIndex);
}

void DeviceContext::drawIndexedLineList(UINT indexCount, UINT startVertexIndex, UINT startIndexIndex) {
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
	deviceContext->DrawIndexed(indexCount, startIndexIndex, startVertexIndex);
}

void DeviceContext::drawInstanced(UINT vertexCount, UINT instanceCount) {
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	deviceContext->DrawInstanced(vertexCount, instanceCount, 0, 0);
}

void DeviceContext::drawIndexedInstanced(UINT indexCount, UINT instanceCount) {
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	deviceContext->DrawIndexedInstanced(indexCount, instanceCount, 0, 0, 0);
}

void DeviceContext::setVertexBuffer(const VertexBufferPtr& vertexBuffer) {
	UINT stride = vertexBuffer->sizeVertex;
	UINT offset = 0;
	deviceContext->IASetVertexBuffers(0, 1, &vertexBuffer->buffer, &stride, &offset);
	deviceContext->IASetInputLayout(vertexBuffer->layout);
}

void DeviceContext::setVertexBuffer(const VertexBufferPtr& vertexBuffer, UINT* stride, UINT* offset) {
	deviceContext->IASetVertexBuffers(0, 1, &vertexBuffer->buffer, stride, offset);
	deviceContext->IASetInputLayout(vertexBuffer->layout);
}

void DeviceContext::setVertexAndInstanceBuffer(const VertexBufferPtr& vertexBuffer, const InstanceBufferPtr& instanceBuffer) {
	ID3D11Buffer* array[2] = {vertexBuffer->buffer, instanceBuffer->buffer};
	UINT strides[2] = {vertexBuffer->sizeVertex, instanceBuffer->dataSize};
	UINT offsets[2] = {0, 0};
	deviceContext->IASetVertexBuffers(0, 2, array, strides, offsets);
	deviceContext->IASetInputLayout(vertexBuffer->layout);
}

void DeviceContext::setIndexBuffer(const IndexBufferPtr& indexBuffer) {
	deviceContext->IASetIndexBuffer(indexBuffer->buffer, DXGI_FORMAT_R32_UINT, 0);
}

void DeviceContext::setVSConstantBuffer(UINT slot, const ConstantBufferPtr& constantBuffer) {
	deviceContext->VSSetConstantBuffers(slot, 1, &constantBuffer->buffer);
}

void DeviceContext::setPSConstantBuffer(UINT slot, const ConstantBufferPtr& constantBuffer) {
	deviceContext->PSSetConstantBuffers(slot, 1, &constantBuffer->buffer);
}

void DeviceContext::setVertexShader(const VertexShaderPtr& vertexShader) {
	deviceContext->VSSetShader(vertexShader->vertexShader, nullptr, 0);
}

void DeviceContext::setPixelShader(const PixelShaderPtr& pixelShader){
	deviceContext->PSSetShader(pixelShader->pixelShader, nullptr, 0);
}

void DeviceContext::setVSTexture(UINT slot, const TexturePtr& texture) {
	deviceContext->VSSetShaderResources(slot, 1, &texture->shaderResourceView);
}

void DeviceContext::setPSTexture(UINT slot, const TexturePtr& texture) {
	deviceContext->PSSetShaderResources(slot, 1, &texture->shaderResourceView);
}

void DeviceContext::setVSTextureArray(UINT slot, const TextureArrayPtr& texture, UINT textureCount) {
	deviceContext->VSSetShaderResources(0, textureCount, texture->getTextures());
}

void DeviceContext::setPSTextureArray(UINT slot, const TextureArrayPtr& texture, UINT textureCount) {
	deviceContext->PSSetShaderResources(0, textureCount, texture->getTextures());
}
