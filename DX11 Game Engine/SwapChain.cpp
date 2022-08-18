#include "SwapChain.h"

#include <exception>

#include "RenderSystem.h"

SwapChain::SwapChain(HWND hwnd, UINT width, UINT height, RenderSystem* renderSystem) : renderSystem(renderSystem) {
	ID3D11Device* device = renderSystem->d3dDevice;

	DXGI_SWAP_CHAIN_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	desc.BufferCount = 1;
	desc.BufferDesc.Width = width;
	desc.BufferDesc.Height = height;
	desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	desc.BufferDesc.RefreshRate.Denominator = 1;
	desc.BufferDesc.RefreshRate.Numerator = 60;
	desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	desc.OutputWindow = hwnd;
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
	desc.Windowed = true;

	// create swapchain for the window specified
	HRESULT result = renderSystem->dxgiFactory->CreateSwapChain(device, &desc, &swapchain);

	if (FAILED(result)) {
		throw std::exception("SwapChain could not be created.");
	}

	reloadBuffers(width, height);
}

SwapChain::~SwapChain() {
	depthStencilView->Release();
	renderTargetView->Release();
	swapchain->Release();
}

void SwapChain::resize(unsigned int width, unsigned int height) {
	if (renderTargetView) {
		renderTargetView->Release();
	}

	if (depthStencilView) {
		depthStencilView->Release();
	}

	swapchain->ResizeBuffers(1, width, height, DXGI_FORMAT_R8G8B8A8_UNORM, 0);
	reloadBuffers(width, height);
}

void SwapChain::setFullscreen(bool fullscreen, unsigned int width, unsigned int height) {
	resize(width, height);
	swapchain->SetFullscreenState(fullscreen, nullptr);
}

bool SwapChain::present(bool vsync) {
	swapchain->Present(vsync, NULL);
	return true;
}

void SwapChain::reloadBuffers(unsigned int width, unsigned int height) {
	ID3D11Device* device = renderSystem->d3dDevice;
	
	ID3D11Texture2D* buffer;
	HRESULT result = swapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&buffer);

	if (FAILED(result)) {
		throw std::exception("Could not get SwapChain buffer.");
	}

	result = device->CreateRenderTargetView(buffer, NULL, &renderTargetView);
	buffer->Release();

	if (FAILED(result)) {
		throw std::exception("RenderTargetView could not be created.");
	}

	D3D11_TEXTURE2D_DESC tex_desc = {};
	tex_desc.Width = width;
	tex_desc.Height = height;
	tex_desc.Format = DXGI_FORMAT_D32_FLOAT;
	tex_desc.Usage = D3D11_USAGE_DEFAULT;
	tex_desc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	tex_desc.MipLevels = 1;
	tex_desc.SampleDesc.Count = 1;
	tex_desc.SampleDesc.Quality = 0;
	tex_desc.MiscFlags = 0;
	tex_desc.ArraySize = 1;
	tex_desc.CPUAccessFlags = 0;

	result = device->CreateTexture2D(&tex_desc, nullptr, &buffer);

	if (FAILED(result)) {
		throw std::exception("RenderTargetView could not be created.");
	}

	result = device->CreateDepthStencilView(buffer, nullptr, &depthStencilView);
	buffer->Release();

	if (FAILED(result)) {
		throw std::exception("DepthStencilView could not be created.");
	}
}
