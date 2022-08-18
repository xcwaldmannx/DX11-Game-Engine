#pragma once

#include <d3d11.h>
#include "Prerequisites.h"

class SwapChain {
public:
	SwapChain(HWND hwnd, UINT width, UINT height, RenderSystem* renderSystem);
	~SwapChain();

	void resize(unsigned int width, unsigned int height);
	void setFullscreen(bool fullscreen, unsigned int width, unsigned int height);

	bool present(bool vsync);
private:
	void reloadBuffers(unsigned int width, unsigned int height);

private:
	RenderSystem* renderSystem = nullptr;

	IDXGISwapChain* swapchain = nullptr;
	ID3D11RenderTargetView* renderTargetView = nullptr;
	ID3D11DepthStencilView* depthStencilView = nullptr;

	friend class DeviceContext;
};

