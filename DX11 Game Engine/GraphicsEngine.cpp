#include "GraphicsEngine.h"

#include <exception>

#include "DeviceContext.h"
#include "RenderSystem.h"

GraphicsEngine* GraphicsEngine::engine = nullptr;

GraphicsEngine::GraphicsEngine() {
    try {
        renderSystem = new RenderSystem();
    } catch (...) {
        throw std::exception("RenderSystem could not be created.");
    }

    try {
        textureManager = new TextureManager();
    }
    catch (...) {
        throw std::exception("TextureManager could not be created.");
    }
    
    try {
        meshManager = new MeshManager();
    }
    catch (...) {
        throw std::exception("MeshManager could not be created.");
    }

    void* shaderByteCode = nullptr;
    size_t sizeShaderByteCode = 0;
    renderSystem->compileVertexShader(L"VertexLayoutShader.hlsl", "main", &shaderByteCode, &sizeShaderByteCode);
    memcpy(meshLayoutByteCode, shaderByteCode, sizeShaderByteCode);
    meshLayoutSize = sizeShaderByteCode;
    renderSystem->releaseCompiledShader();

}

GraphicsEngine::~GraphicsEngine() {
    engine = nullptr;
    delete meshManager;
    delete textureManager;
    delete renderSystem;
}

void GraphicsEngine::create() {
    if (GraphicsEngine::engine) {
        throw std::exception("GraphicsEngine already exists.");
    }
    GraphicsEngine::engine = new GraphicsEngine();
}

void GraphicsEngine::release() {
    if (!GraphicsEngine::engine) {
        return;
    }
    delete GraphicsEngine::engine;
}

GraphicsEngine* GraphicsEngine::get() {
    return engine;
}

RenderSystem* GraphicsEngine::getRenderSystem() {
    return renderSystem;
}

TextureManager* GraphicsEngine::getTextureManager() {
    return textureManager;
}

MeshManager* GraphicsEngine::getMeshManager() {
    return meshManager;
}

MaterialPtr GraphicsEngine::createMaterial(const wchar_t* vertexShaderPath, const wchar_t* pixelShaderPath, unsigned int textureCount) {
    MaterialPtr material = nullptr;
    try {
        material = std::make_shared<Material>(vertexShaderPath, pixelShaderPath, textureCount);
    } catch (...) {}
    return material;
}

void GraphicsEngine::setMaterial(const MaterialPtr& material) {

    GraphicsEngine::get()->getRenderSystem()->setRasterizerState(material->cullMode);

    GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setVertexShader(material->vertexShader);
    GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setPixelShader(material->pixelShader);

    // GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setVSConstantBuffer(0, material->constantBuffer);
    // GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setPSConstantBuffer(0, material->constantBuffer);
    GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setVSTextureArray(0, material->textures, material->textureCount);
    GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setPSTextureArray(0, material->textures, material->textureCount);
}

void GraphicsEngine::getVertexLayoutShaderData(void** byteCode, size_t* size) {
    *byteCode = meshLayoutByteCode;
    *size = meshLayoutSize;
}
