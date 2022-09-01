#pragma once

#include <d3d11.h>

#include "Prerequisites.h"
#include "RenderSystem.h"
#include "TextureManager.h"
#include "MeshManager.h"

#include "Material.h"

class GraphicsEngine {
public:
    GraphicsEngine();
    ~GraphicsEngine();

    static void create();
    static void release();
    static GraphicsEngine* get();

    RenderSystem* getRenderSystem();
    TextureManager* getTextureManager();
    MeshManager* getMeshManager();

    MaterialPtr createMaterial(const wchar_t* vertexShaderPath, const wchar_t* pixelShaderPath, unsigned int textureCount);
    void setMaterial(const MaterialPtr& material);

    void getVertexLayoutShaderData(void** byteCode, size_t* size);

private:
    static GraphicsEngine* engine;
    RenderSystem* renderSystem = nullptr;
    TextureManager* textureManager = nullptr;
    MeshManager* meshManager = nullptr;

    unsigned char meshLayoutByteCode[1024]{};
    size_t meshLayoutSize = 0;
};
