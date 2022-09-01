#pragma once

#include <d3d11.h>

#include "Prerequisites.h"

#include "DeviceContext.h"
#include "SwapChain.h"

#include "VertexBuffer.h"
#include "InstanceBuffer.h"
#include "IndexBuffer.h"
#include "ConstantBuffer.h"

#include "VertexShader.h"
#include "PixelShader.h"

enum RASTER_MODE {
    RASTER_CULL_FRONT,
    RASTER_CULL_BACK,
    RASTER_CULL_NONE,
    RASTER_WIREFRAME,
};

class RenderSystem {
public:
    RenderSystem();
    ~RenderSystem();

    SwapChainPtr createSwapChain(HWND hwnd, UINT width, UINT height);

    DeviceContextPtr getImmediateDeviceContext();

    VertexBufferPtr createVertexBuffer(void* listVertices, UINT sizeVertex, UINT sizeList, INPUT_LAYOUT layout, void* shaderByteCode, UINT sizeByteShaderCode);
    InstanceBufferPtr createInstanceBuffer(void* instances, UINT dataSize, UINT numInstances, void* shaderByteCode, UINT sizeByteShaderCode);
    IndexBufferPtr createIndexBuffer(void* listIndices, UINT sizeList);
    ConstantBufferPtr createConstantBuffer(void* buffer, UINT sizeBuffer);

    VertexShaderPtr createVertexShader(const void* shaderByteCode, size_t sizeByteCode);
    PixelShaderPtr createPixelShader(const void* shaderByteCode, size_t sizeByteCode);

    bool compileVertexShader(const wchar_t* filename, const char* entryPointName, void** shaderByteCode, size_t* sizeByteCode);
    bool compilePixelShader(const wchar_t* filename, const char* entryPointName, void** shaderByteCode, size_t* sizeByteCode);
    void releaseCompiledShader();

public:
    void setRasterizerState(RASTER_MODE mode);
private:
    void initRasterizerState();
    void initBlendState();
    void initSamplerState();

private:
    D3D_FEATURE_LEVEL featureLevel = D3D_FEATURE_LEVEL_11_0;

    ID3D11Device* d3dDevice = nullptr;
    IDXGIDevice* dxgiDevice = nullptr;
    IDXGIAdapter* dxgiAdapter = nullptr;
    IDXGIFactory* dxgiFactory = nullptr;
    ID3D11DeviceContext* d3dDeviceContext = nullptr;

    friend class SwapChain;
    friend class VertexBuffer;
    friend class InstanceBuffer;
    friend class IndexBuffer;
    friend class ConstantBuffer;
    friend class VertexShader;
    friend class PixelShader;
    friend class Texture;

    DeviceContextPtr deviceContext = nullptr;

    ID3DBlob* blob = nullptr;

    ID3DBlob* vsBlob = nullptr;
    ID3DBlob* psBlob = nullptr;
    ID3D11VertexShader* vs = nullptr;
    ID3D11PixelShader* ps = nullptr;

    ID3D11RasterizerState* cullFrontState = nullptr;
    ID3D11RasterizerState* cullBackState = nullptr;
    ID3D11RasterizerState* cullNoneState = nullptr;

    ID3D11RasterizerState* wireframeState = nullptr;

    ID3D11BlendState* blendState = nullptr;
};
