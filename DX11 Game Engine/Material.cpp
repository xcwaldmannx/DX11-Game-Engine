#include "Material.h"

#include <exception>

#include "GraphicsEngine.h"
#include "TextureArray.h"

Material::Material(const wchar_t* vertexShaderPath, const wchar_t* pixelShaderPath, unsigned int textureCount) : textureCount(textureCount) {

    void* shaderByteCode = nullptr;
    size_t sizeShaderByteCode = 0;

    GraphicsEngine::get()->getRenderSystem()->compileVertexShader(vertexShaderPath, "main", &shaderByteCode, &sizeShaderByteCode);
    vertexShader = GraphicsEngine::get()->getRenderSystem()->createVertexShader(shaderByteCode, sizeShaderByteCode);
    GraphicsEngine::get()->getRenderSystem()->releaseCompiledShader();

    if (!vertexShader) {
        throw std::exception("Material could not be created.");
    }

    GraphicsEngine::get()->getRenderSystem()->compilePixelShader(pixelShaderPath, "main", &shaderByteCode, &sizeShaderByteCode);
    pixelShader = GraphicsEngine::get()->getRenderSystem()->createPixelShader(shaderByteCode, sizeShaderByteCode);
    GraphicsEngine::get()->getRenderSystem()->releaseCompiledShader();

    if (!pixelShader) {
        throw std::exception("Material could not be created.");
    }
}

Material::~Material() {
}

void Material::addTexture(const TexturePtr& texture) {
    if (!textures) {
        textures = GraphicsEngine::get()->getTextureManager()->createTextureArray(textureCount);
    }
    textures->addTexture(texture);
}

void Material::removeTexture(unsigned int index) {
    if (index > textureCount) {
        return;
    }
    textures->getTextures()[index] = nullptr;
}

void Material::setData(void* data, unsigned int size) {
    if (!constantBuffer) {
        constantBuffer = GraphicsEngine::get()->getRenderSystem()->createConstantBuffer(data, size);
    } else {
        constantBuffer->update(GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext(), data);
    }
}

void Material::setCullMode(RASTER_MODE mode) {
    cullMode = mode;
}

RASTER_MODE Material::getCullMode() {
    return cullMode;
}
