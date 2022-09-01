#include "DrawableSystem.h"

#include "GraphicsEngine.h"
#include "DeviceContext.h"

//#include "IndexBuffer.h"

extern Coordinator coordinator;

void DrawableSystem::render() {
    for (auto const& entity : entities) {
        auto& drawable = coordinator.getComponent<DrawableComponent>(entity);

        // Set shaders
        GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setVertexShader(drawable.vertexShader);
        GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setPixelShader(drawable.pixelShader);

        // Update and set constant buffers

        for (CONSTANT_BUFFER_SLOT slot : drawable.slotsInUse) {
            ConstantBufferPtr ptr = drawable.constantBufferPtrs[slot];
            auto& data = drawable.constantBufferData[slot];
            ptr->update(GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext(), data);
            GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setVSConstantBuffer(slot, ptr);
            GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setPSConstantBuffer(slot, ptr);
        }

        // Set textures
        for (int i = 0; i < drawable.vertexShaderTextures.size(); i++) {
            GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setVSTexture(i, drawable.vertexShaderTextures[i]);
        }

        for (int i = 0; i < drawable.pixelShaderTextures.size(); i++) {
            GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setPSTexture(i, drawable.pixelShaderTextures[i]);
        }

        // Set vertex and index buffer
        GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setVertexBuffer(drawable.mesh->getVertexBuffer());
        GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setIndexBuffer(drawable.mesh->getIndexBuffer());

        // Draw command
        GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->drawIndexedTriangleList(drawable.mesh->getIndexBuffer()->getSizeIndexList(), 0, 0);

        // AABB WIREFRAME
        if (drawable.boundingboxShader) {
            GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setPixelShader(drawable.boundingboxShader);
            GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setVertexBuffer(drawable.mesh->getVertexBufferBB());
            GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setIndexBuffer(drawable.mesh->getIndexBufferBB());
            GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->drawIndexedLineList(drawable.mesh->getIndexBufferBB()->getSizeIndexList(), 0, 0);
        }

    }
}
