#include "RenderSystem.h"

#include <exception>

#include <d3dcompiler.h>

class VertexBuffer;

RenderSystem::RenderSystem() {
    D3D_DRIVER_TYPE driverTypes[] = {
       D3D_DRIVER_TYPE_HARDWARE,
       D3D_DRIVER_TYPE_WARP,
       D3D_DRIVER_TYPE_REFERENCE };
    UINT numDriverTypes = ARRAYSIZE(driverTypes);

    D3D_FEATURE_LEVEL featureLevels[] = {
        D3D_FEATURE_LEVEL_11_0 };
    UINT numFeatureLevels = ARRAYSIZE(featureLevels);

    HRESULT result = 0;

    for (UINT driverTypeIndex = 0; driverTypeIndex < numDriverTypes; driverTypeIndex++) {
        result = D3D11CreateDevice(NULL, driverTypes[driverTypeIndex], NULL, NULL,
            featureLevels, numFeatureLevels, D3D11_SDK_VERSION, &d3dDevice, &featureLevel, &d3dDeviceContext);
        if (SUCCEEDED(result)) {
            break;
        }
    }

    if (FAILED(result)) {
        throw std::exception("RenderSystem could not be created.");
    }

    deviceContext = std::make_shared<DeviceContext>(d3dDeviceContext, this);

    d3dDevice->QueryInterface(__uuidof(IDXGIDevice), (void**)&dxgiDevice);
    dxgiDevice->GetParent(__uuidof(IDXGIAdapter), (void**)&dxgiAdapter);
    dxgiAdapter->GetParent(__uuidof(IDXGIFactory), (void**)&dxgiFactory);

    initRasterizerState();
    initBlendState();
    initSamplerState();

}

RenderSystem::~RenderSystem() {
    if (vs)vs->Release();
    if (ps)ps->Release();

    if (vsBlob)vsBlob->Release();
    if (psBlob)psBlob->Release();

    dxgiDevice->Release();
    dxgiAdapter->Release();
    dxgiFactory->Release();

    d3dDevice->Release();
}

SwapChainPtr RenderSystem::createSwapChain(HWND hwnd, UINT width, UINT height) {
    SwapChainPtr swapchain = nullptr;
    try {
        swapchain = std::make_shared<SwapChain>(hwnd, width, height, this);
    }
    catch (...) {}
    return swapchain;
}

DeviceContextPtr RenderSystem::getImmediateDeviceContext() {
    return deviceContext;
}

VertexBufferPtr RenderSystem::createVertexBuffer(void* listVertices, UINT sizeVertex, UINT sizeList) {
    VertexBufferPtr vbuffer = nullptr;
    try {
        vbuffer = std::make_shared<VertexBuffer>(listVertices, sizeVertex, sizeList, this);
    } catch (...) {}
    return vbuffer;
}

InstanceBufferPtr RenderSystem::createInstanceBuffer(void* instances, UINT dataSize, UINT numInstances) {
    InstanceBufferPtr ibuffer = nullptr;
    try {
        ibuffer = std::make_shared<InstanceBuffer>(instances, dataSize, numInstances, this);
    }
    catch (...) {}
    return ibuffer;
}

InputLayoutPtr RenderSystem::createInputLayout(const std::vector<D3D11_INPUT_ELEMENT_DESC>& elements, const wchar_t* shaderFilename, const char* shaderEntrypoint) {
    InputLayoutPtr layout = nullptr;
    try {
        layout = std::make_shared<InputLayout>(elements, shaderFilename, shaderEntrypoint, this);
    }
    catch (...) {}
    return layout;
}

IndexBufferPtr RenderSystem::createIndexBuffer(void* listIndices, UINT sizeList) {
    IndexBufferPtr ibuffer = nullptr;
    try {
        ibuffer = std::make_shared<IndexBuffer>(listIndices, sizeList, this);
    } catch (...) {}
    return ibuffer;
}

ConstantBufferPtr RenderSystem::createConstantBuffer(void* buffer, UINT sizeBuffer) {
    ConstantBufferPtr cbuffer = nullptr;
    try {
        cbuffer = std::make_shared<ConstantBuffer>(buffer, sizeBuffer, this);
    } catch(...) {}
    return cbuffer;
}

VertexShaderPtr RenderSystem::createVertexShader(const void* shaderByteCode, size_t sizeByteCode) {
    VertexShaderPtr vshader = nullptr;
    try {
        vshader = std::make_shared<VertexShader>(shaderByteCode, sizeByteCode, this);
    } catch(...) {}
    return vshader;
}

PixelShaderPtr RenderSystem::createPixelShader(const void* shaderByteCode, size_t sizeByteCode) {
    PixelShaderPtr pshader = nullptr;
    try {
        pshader = std::make_shared<PixelShader>(shaderByteCode, sizeByteCode, this);
    } catch (...) {}
    return pshader;
}

bool RenderSystem::compileVertexShader(const wchar_t* filename, const char* entryPointName, void** shaderByteCode, size_t* sizeByteCode) {
    ID3DBlob* errorBlob;
    HRESULT result = D3DCompileFromFile(filename, nullptr, nullptr, entryPointName, "vs_5_0", 0, 0, &blob, &errorBlob);
    if (FAILED(result)) {
        if (errorBlob) errorBlob->Release();
        return false;
    }

    *shaderByteCode = blob->GetBufferPointer();
    *sizeByteCode = blob->GetBufferSize();

    return true;
}

bool RenderSystem::compilePixelShader(const wchar_t* filename, const char* entryPointName, void** shaderByteCode, size_t* sizeByteCode) {
    ID3DBlob* errorBlob;
    HRESULT result = D3DCompileFromFile(filename, nullptr, nullptr, entryPointName, "ps_5_0", 0, 0, &blob, &errorBlob);
    if (FAILED(result)) {
        if (errorBlob) errorBlob->Release();
        return false;
    }

    *shaderByteCode = blob->GetBufferPointer();
    *sizeByteCode = blob->GetBufferSize();

    return true;
}

void RenderSystem::releaseCompiledShader() {
    if (blob) blob->Release();
}

void RenderSystem::setRasterizerState(RASTER_MODE mode) {
    switch (mode) {
    case RASTER_CULL_FRONT:
        d3dDeviceContext->RSSetState(cullFrontState);
        break;
    case RASTER_CULL_BACK:
        d3dDeviceContext->RSSetState(cullBackState);
        break;
    case RASTER_CULL_NONE:
        d3dDeviceContext->RSSetState(cullNoneState);
        break;
    case RASTER_WIREFRAME:
        d3dDeviceContext->RSSetState(wireframeState);
        break;
    default:
        d3dDeviceContext->RSSetState(cullBackState);
        break;
    }
}

void RenderSystem::initRasterizerState() {
    D3D11_RASTERIZER_DESC desc = {};

    desc.DepthClipEnable = true;
    desc.MultisampleEnable = true;
    desc.FillMode = D3D11_FILL_WIREFRAME;
    desc.CullMode = D3D11_CULL_BACK;
    d3dDevice->CreateRasterizerState(&desc, &wireframeState);
    desc.FillMode = D3D11_FILL_SOLID;
    d3dDevice->CreateRasterizerState(&desc, &cullBackState);

    desc.CullMode = D3D11_CULL_FRONT;
    d3dDevice->CreateRasterizerState(&desc, &cullFrontState);

    desc.CullMode = D3D11_CULL_NONE;
    d3dDevice->CreateRasterizerState(&desc, &cullNoneState);
}

void RenderSystem::initBlendState() {

    D3D11_BLEND_DESC desc;
    desc.AlphaToCoverageEnable = false;
    desc.IndependentBlendEnable = false;
    desc.RenderTarget[0].BlendEnable = true;
    desc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
    desc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
    desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
    desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
    desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
    desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
    desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

    d3dDevice->CreateBlendState(&desc, &blendState);
    d3dDeviceContext->OMSetBlendState(blendState, 0, 0xffffffff);

}

void RenderSystem::initSamplerState() {
    ID3D11SamplerState* m_sampleState;
    D3D11_SAMPLER_DESC samplerDesc;
    samplerDesc.Filter = D3D11_FILTER_ANISOTROPIC;
    samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
    samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
    samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
    samplerDesc.MipLODBias = 0.0f;
    samplerDesc.MaxAnisotropy = D3D11_MAX_MAXANISOTROPY;
    samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
    samplerDesc.BorderColor[0] = 0;
    samplerDesc.BorderColor[1] = 0;
    samplerDesc.BorderColor[2] = 0;
    samplerDesc.BorderColor[3] = 0;
    samplerDesc.MinLOD = 0.0f;
    samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

    // Create the texture sampler state.
    HRESULT result = d3dDevice->CreateSamplerState(&samplerDesc, &m_sampleState);
    d3dDeviceContext->PSSetSamplers(0, 1, &m_sampleState);
}
