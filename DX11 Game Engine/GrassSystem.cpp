#include "GrassSystem.h"

#include "GraphicsEngine.h"
#include "ConstantBufferTypes.h"

#include "Mesh.h"

GrassSystem::GrassSystem(unsigned int numGrassBlades, LODTerrain* terrain) : numGrassBlades(numGrassBlades), terrain(terrain) {
	mesh = GraphicsEngine::get()->getMeshManager()->createMeshFromFile(L"Assets/Meshes/grasstuft.obj");
	texture = GraphicsEngine::get()->getTextureManager()->createTextureFromFile(L"Assets/Textures/grass_tuft.png");

	// instance data
	int length = sqrt(numGrassBlades);
	for (unsigned int i = 0; i < length; i++) {
		for (unsigned int j = 0; j < length; j++) {
			float x = (float) i / 5.0f;
			float z = (float) j / 5.0f;
			GrassInstance inst = {
				Vec3f(x, terrain->getHeightAt(x, z), z),
				Vec3f((float) i / (float) length, 1, 1),
			};
			instances.push_back(inst);
		}
	}

	std::vector<InputElement> elements = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,  0, D3D11_INPUT_PER_VERTEX_DATA,   0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,    0, 12, D3D11_INPUT_PER_VERTEX_DATA,   0 },
		{ "NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 20, D3D11_INPUT_PER_VERTEX_DATA,   0 },

		{ "POSITION", 1, DXGI_FORMAT_R32G32B32_FLOAT, 1,  0, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
		{ "COLOR",    0, DXGI_FORMAT_R32G32B32_FLOAT, 1, 12, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
	};

	inputLayout = GraphicsEngine::get()->getRenderSystem()->createInputLayout(elements, L"GrassVertexShader.hlsl", "main");
	vertexBuffer = GraphicsEngine::get()->getRenderSystem()->createVertexBuffer(&mesh->getVertices()[0], sizeof(Vertex), (UINT)mesh->getVertices().size());
	instanceBuffer = GraphicsEngine::get()->getRenderSystem()->createInstanceBuffer(&instances[0], sizeof(GrassInstance), (UINT)instances.size());
	indexBuffer = GraphicsEngine::get()->getRenderSystem()->createIndexBuffer(&mesh->getIndices()[0], mesh->getIndices().size());
}

GrassSystem::~GrassSystem() {
}

const InputLayoutPtr& GrassSystem::getInputLayout() {
	return inputLayout;
}

const VertexBufferPtr& GrassSystem::getVertexBuffer() {
	return vertexBuffer;
}

const InstanceBufferPtr& GrassSystem::getInstanceBuffer() {
	return instanceBuffer;
}

const IndexBufferPtr& GrassSystem::getIndexBuffer() {
	return indexBuffer;
}

const TexturePtr& GrassSystem::getTexture() {
	return texture;
}

const UINT GrassSystem::getVertexCount() {
	return mesh->getVertices().size();
}

const UINT GrassSystem::getIndexCount() {
	return mesh->getIndices().size();
}
