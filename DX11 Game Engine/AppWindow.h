#pragma once

#include <chrono>
#include <vector>

#include "Window.h"
#include "GraphicsEngine.h"

#include "SwapChain.h"
#include "DeviceContext.h"

#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "ConstantBuffer.h"

#include "VertexShader.h"
#include "PixelShader.h"

#include "Mat4f.h"

#include "TerrainManager.h"

#include "Camera.h"

#include "ECS.h"

#include "TextureArray.h"

#include "InputManager.h"

class AppWindow : public Window {
public:
    AppWindow();
    ~AppWindow();

    void draw();
    void update();
    void updateSkybox();
    void updateTerrain();
    void updateModel(Vec3f position);

    void drawMesh(const MeshPtr& mesh, const std::vector<MaterialPtr>& materials, ConstantBufferPtr cBuffer[]);

    void placeEntity();

    // window overrides
    virtual void onCreate() override;
    virtual void onUpdate() override;
    virtual void onDestroy() override;
    virtual void onFocus() override;
    virtual void onKillFocus() override;
    virtual void onSize() override;

    // input
    Point2f getWindowCenter();
    void updateInputEvents();

private:
    SwapChainPtr swapchain = nullptr;
    VertexBufferPtr vertexBuffer = nullptr;

    // Constant Buffers
    ConstantBufferPtr transformBuffer = nullptr;
    ConstantBufferPtr lightingBuffer = nullptr;
    ConstantBufferPtr pointLightBuffer = nullptr;

    // Vertex Shader
    VertexShaderPtr vertexShader = nullptr;

    // Pixel Shaders
    PixelShaderPtr basicPS = nullptr;
    PixelShaderPtr skyboxPS = nullptr;
    PixelShaderPtr terrainPS = nullptr;
    PixelShaderPtr pixelationPS = nullptr;
    PixelShaderPtr pointLightPS = nullptr;
    PixelShaderPtr boundingBoxShader = nullptr;

    // Terrain
    TerrainManager* terrainManager = nullptr;

    TexturePtr grassTexture = nullptr, dirtTexture = nullptr, pathTexture = nullptr, pathmapTexture = nullptr, heightmapTexture = nullptr;
    TextureArrayPtr terrainTextures = nullptr;

    // Materials list
    std::vector<MaterialPtr> materialsList{};

    // Skybox
    TexturePtr sky = nullptr;
    MeshPtr skyMesh = nullptr;
    MaterialPtr skyMaterial = nullptr;

    // pine tree
    MeshPtr pinetreeMesh = nullptr;
    TexturePtr branchTexture = nullptr;
    MaterialPtr branchMaterial = nullptr;

    // dead tree
    MeshPtr deadTreeMesh = nullptr;
    TexturePtr deadTreeTex = nullptr;

    //scene
    MeshPtr sceneMesh = nullptr;

    TexturePtr barkTexture = nullptr;
    TexturePtr boxTexture = nullptr;

    MaterialPtr barkMaterial = nullptr;
    MaterialPtr boxMaterial = nullptr;

    // timing
    long double deltaTime = 0.0;
    std::chrono::steady_clock::time_point lastUpdate{};

    // mouse clicking
    int click = -1;

    bool canPlaceEntity = true;

    // camera rotation
    float rotX = 0.0f;
    float rotY = 0.0f;

    // camera direction
    bool flyingMode = false;
    float forward = 0.0f;
    float right = 0.0f;
    float up = 0.0f;

    // camera matrices
    Camera camera{nullptr};

    // play state or non-play state indication
    bool playState = false;
    // set fullscreen
    bool fullscreen = false;

    // Entities
    ECS ecs{};

    // Input    
    InputManager* inputManager = new InputManager();        

};
