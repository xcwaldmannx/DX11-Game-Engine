#include "DrawableSystem.h"

#include "Coordinator.h"
#include "ComponentTypes.h"

#include "GraphicsEngine.h"
#include "DeviceContext.h"

#include "IndexBuffer.h"
#include "ConstantBuffer.h"


extern Coordinator coordinator;

void DrawableSystem::render() {
    for (auto const& entity : entities) {
        auto& drawable = coordinator.getComponent<DrawableComponent>(entity);

        // Set shaders
        GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setVertexShader(drawable.VertexShader);
        GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setPixelShader(drawable.PixelShader);

        // Update and set constant buffers

        for (CONSTANT_BUFFER_SLOT slot : drawable.SlotsInUse) {
            ConstantBufferPtr ptr = drawable.constantBufferPtrs[slot];
            auto& data = drawable.constantBufferData[slot];
            ptr->update(GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext(), data);
            GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setVSConstantBuffer(slot, ptr);
            GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setPSConstantBuffer(slot, ptr);
        }

        // Set textures
        for (int i = 0; i < drawable.VertexShaderTextures.size(); i++) {
            GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setVSTexture(i, drawable.VertexShaderTextures[i]);
        }

        for (int i = 0; i < drawable.PixelShaderTextures.size(); i++) {
            GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setPSTexture(i, drawable.PixelShaderTextures[i]);
        }

        // Set vertex and index buffer
        GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setVertexBuffer(drawable.Mesh->getVertexBuffer());
        GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setIndexBuffer(drawable.Mesh->getIndexBuffer());

        // Draw command
        GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->drawIndexedTriangleList(drawable.Mesh->getIndexBuffer()->getSizeIndexList(), 0, 0);

        // AABB WIREFRAME
        if (drawable.BoundingBoxShader) {
            GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setPixelShader(drawable.BoundingBoxShader);
            GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setVertexBuffer(drawable.Mesh->getVertexBufferBB());
            GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setIndexBuffer(drawable.Mesh->getIndexBufferBB());
            GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->drawWireframe(drawable.Mesh->getIndexBufferBB()->getSizeIndexList(), 0, 0);
        }

    }
}
