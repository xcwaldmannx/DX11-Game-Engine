#pragma once

#include <memory>

class SwapChain;
class DeviceContext;
class VertexBuffer;
class InstanceBuffer;
class InputLayout;
class IndexBuffer;
class ConstantBuffer;
class VertexShader;
class PixelShader;
class RenderSystem;
class GraphicsEngine;
class Resource;
class ResourceManager;
class Texture;
class TextureArray;
class TextureManager;
class Mesh;
class MeshManager;
class Material;

typedef std::shared_ptr<SwapChain> SwapChainPtr;
typedef std::shared_ptr<DeviceContext> DeviceContextPtr;
typedef std::shared_ptr<VertexBuffer> VertexBufferPtr;
typedef std::shared_ptr<InstanceBuffer> InstanceBufferPtr;
typedef std::shared_ptr<InputLayout> InputLayoutPtr;
typedef std::shared_ptr<IndexBuffer> IndexBufferPtr;
typedef std::shared_ptr<ConstantBuffer> ConstantBufferPtr;
typedef std::shared_ptr<VertexShader> VertexShaderPtr;
typedef std::shared_ptr<PixelShader> PixelShaderPtr;
typedef std::shared_ptr<Resource> ResourcePtr;
typedef std::shared_ptr<Texture> TexturePtr;
typedef std::shared_ptr<TextureArray> TextureArrayPtr;
typedef std::shared_ptr<Mesh> MeshPtr;
typedef std::shared_ptr<Material> MaterialPtr;
