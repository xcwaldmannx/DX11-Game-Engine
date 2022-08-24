#include "AppWindow.h"

#include <windows.h>
#include <iostream>

#include "Vec2f.h"
#include "Vec3f.h"
#include "Mat4f.h"

#include "InputSystem.h"
#include "Mesh.h"

#include "ComponentTypes.h"
#include "ConstantBufferTypes.h"

#include "EntityTypes.h"

struct vertex {
    Vec3f position;
    Vec2f texcoord;
};

__declspec(align(16))
struct constant {
    Mat4f world;
    Mat4f view;
    Mat4f proj;
    Vec4f lightDirection;
    Vec4f cameraPosition;
};

AppWindow::AppWindow() {
}

AppWindow::~AppWindow() {
}

std::vector<Vec3f> positions;

void AppWindow::draw() {
    auto now = std::chrono::steady_clock::now();

    // clear window color to specified
    GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->clearRenderTargetColor(swapchain, 0.5f, 0.0f, 1.0f, 1.0f);

    // set viewport dimensions
    RECT dim = getWindowRect();
    GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setViewportSize(dim.right - dim.left, dim.bottom - dim.top);

    // compute transform matrices
    update();

    // Skybox
    updateSkybox();
    ConstantBufferPtr arr[] = { transformBuffer , lightingBuffer };
    materialsList.clear();
    materialsList.push_back(skyMaterial);
    drawMesh(skyMesh, materialsList, arr);


    GraphicsEngine::get()->getRenderSystem()->setRasterizerState(CULL_BACK);

    // draw terrain
    updateTerrain();
    GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setVertexShader(vertexShader);
    GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setPixelShader(terrainPS);

    GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setVSConstantBuffer(0, transformBuffer);
    GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setVSConstantBuffer(1, lightingBuffer);
    GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setPSConstantBuffer(0, transformBuffer);
    GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setPSConstantBuffer(1, lightingBuffer);

    GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setPSTextureArray(0, terrainTextures, terrainTextures->size());

    auto chunks = terrainManager->getChunks();

    for (int i = 0; i < chunks.size(); i++) {
        auto chunk = chunks[i];
        if (chunk->getVertexBuffer() && chunk->getIndexBuffer()) {
            GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setVertexBuffer(chunk->getVertexBuffer());
            GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setIndexBuffer(chunk->getIndexBuffer());

            GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->drawIndexedTriangleList(chunk->getIndexBuffer()->getSizeIndexList(), 0, 0);
        }
    }

    // Scene
    updateModel(camera.getMousePosition());
    ConstantBufferPtr arr1[] = { transformBuffer , lightingBuffer };
    materialsList.clear();
    materialsList.push_back(branchMaterial);
    materialsList.push_back(barkMaterial);
    drawMesh(pinetreeMesh, materialsList, arr1);

    // pine tree
    for (int i = 0; i < 256; i++) {
        updateModel(positions[i]);
        ConstantBufferPtr arr2[] = { transformBuffer , lightingBuffer };
        materialsList.clear();
        materialsList.push_back(branchMaterial);
        materialsList.push_back(barkMaterial);
        drawMesh(pinetreeMesh, materialsList, arr2);
    }
    

    // draw entities
    ecs.draw();

    // show swapchain
    swapchain->present(true);

    deltaTime = std::chrono::duration_cast<std::chrono::nanoseconds>(now - lastUpdate).count() / 1000000000.0;
    lastUpdate = now;

}

void AppWindow::update() {
    updateInputEvents();

    if (canPlaceEntity) {
        placeEntity();
    }

    camera.updateView(this, deltaTime);
    ecs.update(deltaTime);
}

void AppWindow::updateSkybox() {
    // constant buffer update
    TransformBuffer transBuffer;

    transBuffer.world.setIdentity();
    transBuffer.world.setScale(Vec3f(4000.0f, 4000.0f, 4000.0f));
    transBuffer.world.setTranslation(camera.getWorld().getTranslation());
    transBuffer.view = camera.getView();
    transBuffer.proj = camera.getProj();

    transformBuffer->update(GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext(), &transBuffer);
}

void AppWindow::updateTerrain() {
    TransformBuffer transBuffer;
    LightingBuffer lightBuffer;

    Mat4f lightRotMatrix;
    lightRotMatrix.setIdentity();
    lightRotMatrix.setRotationX(3.5f);

    transBuffer.world.setIdentity();
    transBuffer.view = camera.getView();
    transBuffer.proj = camera.getProj();

    lightBuffer.lightDirection = lightRotMatrix.getZDirection();
    lightBuffer.ambient = 0.4f;
    lightBuffer.diffuse = 1.0f;
    lightBuffer.specular = 0.0f;
    lightBuffer.shininess = 1.0f;

    transformBuffer->update(GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext(), &transBuffer);
    lightingBuffer->update(GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext(), &lightBuffer);
}

void AppWindow::updateModel(Vec3f position) {
    TransformBuffer transBuffer;
    LightingBuffer lightBuffer;

    Mat4f lightRotMatrix;
    lightRotMatrix.setIdentity();
    lightRotMatrix.setRotationX(3.5f);

    transBuffer.world.setIdentity();
    transBuffer.world.setTranslation(position);
    transBuffer.view = camera.getView();
    transBuffer.proj = camera.getProj();

    lightBuffer.lightDirection = lightRotMatrix.getZDirection();
    lightBuffer.ambient = 0.4f;
    lightBuffer.diffuse = 1.0f;
    lightBuffer.specular = 0.0f;
    lightBuffer.shininess = 0.0f;

    transformBuffer->update(GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext(), &transBuffer);
    lightingBuffer->update(GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext(), &lightBuffer);
}

void AppWindow::drawMesh(const MeshPtr& mesh, const std::vector<MaterialPtr>& materials, ConstantBufferPtr cBuffer[]) {

    // set vertex buffer and index buffer
    GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setVertexBuffer(mesh->getVertexBuffer());
    GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setIndexBuffer(mesh->getIndexBuffer());

    for (unsigned int m = 0; m < mesh->materialSlotCount(); m++) {
        if (m >= materials.size()) {
            break;
        }

        MaterialSlot mat = mesh->getMaterialSlot(m);

        // set material
        GraphicsEngine::get()->setMaterial(materials[m]);

        // set constant buffer
        GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setVSConstantBuffer(0, cBuffer[0]);
        GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setVSConstantBuffer(1, cBuffer[1]);
        GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setPSConstantBuffer(0, cBuffer[0]);
        GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setPSConstantBuffer(1, cBuffer[1]);

        // draw command
        GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->drawIndexedTriangleList((UINT) mat.numIndices, 0, (UINT) mat.startIndex);
    }
}

void AppWindow::placeEntity() {
    if (click == 1 && canPlaceEntity) {
        canPlaceEntity = false;

        Mat4f lightRotMatrix;
        lightRotMatrix.setIdentity();
        lightRotMatrix.setRotationX(3.5f);

        EntityTree tree{};
        tree.transform.position = camera.getMousePosition();
        tree.transform.camera = &camera;
        tree.drawable.vertexShader = vertexShader;
        tree.drawable.pixelShader = basicPS;
        tree.drawable.boundingboxShader = boundingBoxShader;
        tree.drawable.addBuffer(TRANSFORM, transformBuffer);
        tree.drawable.addBuffer(LIGHTING, lightingBuffer);
        tree.drawable.mesh = deadTreeMesh;
        tree.drawable.vertexShaderTextures = std::vector<TexturePtr>{ deadTreeTex };
        tree.drawable.pixelShaderTextures = std::vector<TexturePtr>{ deadTreeTex };
        tree.lighting.lightDirection = lightRotMatrix.getZDirection();
        tree.lighting.cameraPosition = camera.getWorld().getTranslation();

        EntityID entity = ecs.createEntity();
        ecs.addComponentToEntity<TransformComponent>(entity, tree.transform);
        ecs.addComponentToEntity<DrawableComponent>(entity, tree.drawable);
        ecs.addComponentToEntity<LightingComponent>(entity, tree.lighting);
        ecs.addComponentToEntity<PickableComponent>(entity, tree.pickable);
    }
}

void AppWindow::onCreate() {

    Window::onCreate();

    playState = true;
    fullscreen = false;
    InputSystem::get()->addListener(inputManager);
    InputSystem::get()->showCursor(!playState);

    sky = GraphicsEngine::get()->getTextureManager()->createTextureFromFile(L"Assets\\Textures\\sky.jpg");
    skyMesh = GraphicsEngine::get()->getMeshManager()->createMeshFromFile(L"Assets\\Meshes\\sphere.obj");

    grassTexture = GraphicsEngine::get()->getTextureManager()->createTextureFromFile(L"Assets\\Textures\\grass.png");
    dirtTexture = GraphicsEngine::get()->getTextureManager()->createTextureFromFile(L"Assets\\Textures\\dirt.png");
    pathTexture = GraphicsEngine::get()->getTextureManager()->createTextureFromFile(L"Assets\\Textures\\path.png");
    pathmapTexture = GraphicsEngine::get()->getTextureManager()->createTextureFromFile(L"Assets\\Textures\\pathmap2.png");
    heightmapTexture = GraphicsEngine::get()->getTextureManager()->createTextureFromFile(L"Assets\\Textures\\heightmap2.png");

    terrainTextures = GraphicsEngine::get()->getTextureManager()->createTextureArray(5);
    terrainTextures->addTexture(grassTexture);
    terrainTextures->addTexture(dirtTexture);
    terrainTextures->addTexture(pathTexture);
    terrainTextures->addTexture(pathmapTexture);
    terrainTextures->addTexture(heightmapTexture);

    RECT dim = getWindowRect();
    swapchain = GraphicsEngine::get()->getRenderSystem()->createSwapChain(hwnd, dim.right - dim.left, dim.bottom - dim.top);

    // START CREATE SHADERS

    void* shaderByteCode = nullptr;
    size_t sizeShaderByteCode = 0;

    // VERTEX SHADER
    GraphicsEngine::get()->getRenderSystem()->compileVertexShader(L"VertexShader.hlsl", "main", &shaderByteCode, &sizeShaderByteCode);
    vertexShader = GraphicsEngine::get()->getRenderSystem()->createVertexShader(shaderByteCode, sizeShaderByteCode);
    GraphicsEngine::get()->getRenderSystem()->releaseCompiledShader();

    // PIXEL SHADER
    GraphicsEngine::get()->getRenderSystem()->compilePixelShader(L"PixelShader.hlsl", "main", &shaderByteCode, &sizeShaderByteCode);
    basicPS = GraphicsEngine::get()->getRenderSystem()->createPixelShader(shaderByteCode, sizeShaderByteCode);
    GraphicsEngine::get()->getRenderSystem()->releaseCompiledShader();

    // SKYBOX SHADER
    GraphicsEngine::get()->getRenderSystem()->compilePixelShader(L"SkyboxShader.hlsl", "main", &shaderByteCode, &sizeShaderByteCode);
    skyboxPS = GraphicsEngine::get()->getRenderSystem()->createPixelShader(shaderByteCode, sizeShaderByteCode);
    GraphicsEngine::get()->getRenderSystem()->releaseCompiledShader();
    
    // TERRAIN SHADER
    GraphicsEngine::get()->getRenderSystem()->compilePixelShader(L"TerrainShader.hlsl", "main", &shaderByteCode, &sizeShaderByteCode);
    terrainPS = GraphicsEngine::get()->getRenderSystem()->createPixelShader(shaderByteCode, sizeShaderByteCode);
    GraphicsEngine::get()->getRenderSystem()->releaseCompiledShader();

    // PIXELATION SHADER
    GraphicsEngine::get()->getRenderSystem()->compilePixelShader(L"PixelationShader.hlsl", "main", &shaderByteCode, &sizeShaderByteCode);
    pixelationPS = GraphicsEngine::get()->getRenderSystem()->createPixelShader(shaderByteCode, sizeShaderByteCode);
    GraphicsEngine::get()->getRenderSystem()->releaseCompiledShader();

    // POINTLIGHT SHADER
    GraphicsEngine::get()->getRenderSystem()->compilePixelShader(L"PointLightPixelShader.hlsl", "main", &shaderByteCode, &sizeShaderByteCode);
    pointLightPS = GraphicsEngine::get()->getRenderSystem()->createPixelShader(shaderByteCode, sizeShaderByteCode);
    GraphicsEngine::get()->getRenderSystem()->releaseCompiledShader();

    // BOUNDING BOX SHADER
    GraphicsEngine::get()->getRenderSystem()->compilePixelShader(L"BoundingBoxShader.hlsl", "main", &shaderByteCode, &sizeShaderByteCode);
    boundingBoxShader = GraphicsEngine::get()->getRenderSystem()->createPixelShader(shaderByteCode, sizeShaderByteCode);
    GraphicsEngine::get()->getRenderSystem()->releaseCompiledShader();

    // END CREATE SHADERS

    // START CREATE MATERIALS
    skyMaterial = GraphicsEngine::get()->createMaterial(L"VertexShader.hlsl", L"SkyboxShader.hlsl", 1);
    skyMaterial->addTexture(sky);
    skyMaterial->setCullMode(CULL_FRONT);
    // END CREATE MATERIALS

    sceneMesh = GraphicsEngine::get()->getMeshManager()->createMeshFromFile(L"Assets\\Meshes\\scene1.obj");
    pinetreeMesh = GraphicsEngine::get()->getMeshManager()->createMeshFromFile(L"Assets\\Meshes\\pine_tree.obj");

    barkTexture = GraphicsEngine::get()->getTextureManager()->createTextureFromFile(L"Assets\\Textures\\bark.png");
    branchTexture = GraphicsEngine::get()->getTextureManager()->createTextureFromFile(L"Assets\\Textures\\pine_branch.png");
    boxTexture = GraphicsEngine::get()->getTextureManager()->createTextureFromFile(L"Assets\\Textures\\box.png");

    barkMaterial = GraphicsEngine::get()->createMaterial(L"VertexShader.hlsl", L"PixelShader.hlsl", 1);
    barkMaterial->addTexture(barkTexture);
    barkMaterial->setCullMode(CULL_BACK);

    boxMaterial = GraphicsEngine::get()->createMaterial(L"VertexShader.hlsl", L"PixelShader.hlsl", 1);
    boxMaterial->addTexture(boxTexture);
    boxMaterial->setCullMode(CULL_BACK);

    branchMaterial = GraphicsEngine::get()->createMaterial(L"VertexShader.hlsl", L"PixelShader.hlsl", 1);
    branchMaterial->addTexture(branchTexture);
    branchMaterial->setCullMode(CULL_NONE);
    
    // CONSTANT BUFFERS
    TransformBuffer tb;
    LightingBuffer lb;
    PointLightBuffer plb;
    transformBuffer = GraphicsEngine::get()->getRenderSystem()->createConstantBuffer(&tb, sizeof(TransformBuffer));
    lightingBuffer = GraphicsEngine::get()->getRenderSystem()->createConstantBuffer(&lb, sizeof(LightingBuffer));
    pointLightBuffer = GraphicsEngine::get()->getRenderSystem()->createConstantBuffer(&plb, sizeof(PointLightBuffer));

    // Terrain
    terrainManager = new TerrainManager(2, 2, 1024);
    
    // Camera
    camera = Camera{terrainManager};
    camera.getWorld().setTranslation(Vec3f(16, 256, 16));

    for (int i = 0; i < 256; i++) {
        float x = (float) (rand() % 1024);
        float z = (float) (rand() % 1024);
        float y = terrainManager->getHeightAt(x, z);
        positions.push_back(Vec3f(x, y, z));
    }

    // Entities
    ecs.init();

    deadTreeTex = GraphicsEngine::get()->getTextureManager()->createTextureFromFile(L"Assets\\Textures\\bark.png");
    deadTreeMesh= GraphicsEngine::get()->getMeshManager()->createMeshFromFile(L"Assets\\Meshes\\tree.obj");

    TexturePtr lamp = GraphicsEngine::get()->getTextureManager()->createTextureFromFile(L"Assets\\Textures\\lamp.png");
    MeshPtr lampMesh = GraphicsEngine::get()->getMeshManager()->createMeshFromFile(L"Assets\\Meshes\\lamp.obj");

    Mat4f lightRotMatrix;
    lightRotMatrix.setIdentity();
    lightRotMatrix.setRotationX(3.5f);

    EntityPointLight pointLight{};
    pointLight.transform.position = Vec3f(0, 0, 0);
    pointLight.transform.scale = Vec3f(1, 1, 1);
    pointLight.transform.camera = &camera;
    pointLight.drawable.vertexShader = vertexShader;
    pointLight.drawable.pixelShader = pointLightPS;
    pointLight.drawable.addBuffer(TRANSFORM, transformBuffer);
    pointLight.drawable.addBuffer(POINT_LIGHT, pointLightBuffer);
    pointLight.drawable.mesh = deadTreeMesh;
    pointLight.drawable.vertexShaderTextures = std::vector<TexturePtr>{ deadTreeTex };
    pointLight.drawable.pixelShaderTextures = std::vector<TexturePtr>{ deadTreeTex };
    pointLight.pointLight.lightPosition = Vec4f(0, 1, -5, 0);
    pointLight.pointLight.cameraPosition = camera.getWorld().getTranslation();
    pointLight.pointLight.radius = 25.0f;
    EntityID entity = ecs.createEntity();
    ecs.addComponentToEntity<TransformComponent>(entity, pointLight.transform);
    ecs.addComponentToEntity<DrawableComponent>(entity, pointLight.drawable);
    ecs.addComponentToEntity<PointLightComponent>(entity, pointLight.pointLight);

    EntityPlayerHand playerHand{};
    playerHand.transform.position = Vec3f(-0.5f, -0.5f, 0.5f);
    playerHand.transform.rotation = Vec3f(0, PI / 2, 0);
    playerHand.transform.scale = Vec3f(0.25f, 0.25f, 0.25f);
    playerHand.transform.camera = &camera;
    playerHand.transform.attachedToCamera = true;
    playerHand.transform.translateWithCameraXYZ = Vec3f(1, 1, 1);
    playerHand.transform.rotateWithCameraXYZ = Vec3f(0, 1, 0);
    playerHand.drawable.vertexShader = vertexShader;
    playerHand.drawable.pixelShader = basicPS;
    playerHand.drawable.addBuffer(TRANSFORM, transformBuffer);
    playerHand.drawable.mesh = lampMesh;
    playerHand.drawable.vertexShaderTextures = std::vector<TexturePtr>{ lamp };
    playerHand.drawable.pixelShaderTextures = std::vector<TexturePtr>{ lamp };
    entity = ecs.createEntity();
    ecs.addComponentToEntity<TransformComponent>(entity, playerHand.transform);
    ecs.addComponentToEntity<DrawableComponent>(entity, playerHand.drawable);

    for (int i = 0; i < 512; i++) {
        EntityTree tree{};
        float tx = (float) (rand() % 1024);
        float tz = (float) (rand() % 1024);
        float ty = terrainManager->getHeightAt(tx, tz) - 1;
        float ry = (float) (rand() % 4);
        tree.transform.position = Vec3f(tx, ty, tz);
        tree.transform.camera = &camera;
        tree.drawable.vertexShader = vertexShader;
        tree.drawable.pixelShader = basicPS;
        tree.drawable.boundingboxShader = boundingBoxShader;
        tree.drawable.addBuffer(TRANSFORM, transformBuffer);
        tree.drawable.addBuffer(LIGHTING, lightingBuffer);
        tree.drawable.mesh = deadTreeMesh;
        tree.drawable.vertexShaderTextures = std::vector<TexturePtr>{ deadTreeTex };
        tree.drawable.pixelShaderTextures = std::vector<TexturePtr>{ deadTreeTex };
        tree.lighting.lightDirection = lightRotMatrix.getZDirection();
        tree.lighting.cameraPosition = camera.getWorld().getTranslation();

        EntityID entity = ecs.createEntity();
        ecs.addComponentToEntity<TransformComponent>(entity, tree.transform);
        ecs.addComponentToEntity<DrawableComponent>(entity, tree.drawable);
        ecs.addComponentToEntity<LightingComponent>(entity, tree.lighting);
        ecs.addComponentToEntity<PickableComponent>(entity, tree.pickable);
    }

}

void AppWindow::onUpdate() {
    Window::onUpdate();
    InputSystem::get()->update();
    draw();
}

void AppWindow::onDestroy() {
    Window::onDestroy();
    swapchain->setFullscreen(false, 1, 1);
}

void AppWindow::onFocus() {
    InputSystem::get()->addListener(inputManager);
}

void AppWindow::onKillFocus() {
    InputSystem::get()->removeListener(inputManager);
}

void AppWindow::onSize() {
    RECT dim = getWindowRect();
    swapchain->resize(dim.right, dim.bottom);
    draw();
}

Point2f AppWindow::getWindowCenter() {
    RECT windowSize = getWindowRect();
    RECT windowPos = getWindowPosition();

    int width = (int)(windowSize.right - windowSize.left);
    int height = (int)(windowSize.bottom - windowSize.top);

    int middleX = (int)windowPos.right - (width / 2);
    int middleY = (int)windowPos.bottom - (height / 2);

    return Point2f(middleX, middleY);
}

void AppWindow::updateInputEvents() {

    if (!inputManager->getKeyTapState(9)) {
        playState = !playState;
        InputSystem::get()->showCursor(!playState);
        InputSystem::get()->setCursorPosition(getWindowCenter());
    }

    if (!inputManager->getKeyTapState('F')) {
        fullscreen = !fullscreen;
        RECT screenSize = getScreenSize();
        swapchain->setFullscreen(fullscreen, screenSize.right, screenSize.bottom);
        InputSystem::get()->setCursorPosition(getWindowCenter());
    }

    // play state
    if (playState) {
        Point2f windowCenter = getWindowCenter();
        InputSystem::get()->setCursorPosition(windowCenter);

        // flying mode
        if (!inputManager->getKeyTapState('G')) {
            flyingMode = !flyingMode;
        }

        // moving
        forward = inputManager->getKeyHoldState('W') - inputManager->getKeyHoldState('S');
        right = inputManager->getKeyHoldState('D') - inputManager->getKeyHoldState('A');
        up = inputManager->getKeyHoldState(' ') - inputManager->getKeyHoldState(16);

        // looking
        Point2f mouse = inputManager->getMousePosition();
        rotX += (mouse.y - windowCenter.y) * (float) deltaTime * 0.25f;
        rotY += (mouse.x - windowCenter.x) * (float) deltaTime * 0.25f;
        InputSystem::get()->setCursorPosition(windowCenter);

        if (rotX >= 1.57f) {
            rotX = 1.57f;
        }
        else if (rotX <= -1.57f) {
            rotX = -1.57f;
        }

        // update camera
        camera.updateMouse(rotX, rotY);
        camera.updateMovement(forward, right, up, flyingMode);

    }

    // placing entities
    click = 2 * inputManager->getMouseState(InputManager::LMB_STATE) - 1;
    canPlaceEntity = inputManager->getMouseState(InputManager::LMB_STATE);
}
