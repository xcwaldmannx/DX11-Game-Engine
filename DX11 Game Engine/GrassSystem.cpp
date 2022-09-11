#include "GrassSystem.h"

#include "GraphicsEngine.h"
#include "ConstantBufferTypes.h"

#include "Mesh.h"

#include "InputLayoutDescriptor.h"

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

	InputLayoutDescriptor inputLayoutDesc{};
	inputLayoutDesc.addElement(ILD_TYPE_POSITION, 0, 0, ILD_INPUT_VERTEX, 0);
	inputLayoutDesc.addElement(ILD_TYPE_TEXCOORD, 0, 0, ILD_INPUT_VERTEX, 0);
	inputLayoutDesc.addElement(ILD_TYPE_NORMAL,   0, 0, ILD_INPUT_VERTEX, 0);
	inputLayoutDesc.addElement(ILD_TYPE_POSITION, 1, 1, ILD_INPUT_INSTANCE, 1);
	inputLayoutDesc.addElement(ILD_TYPE_COLOR,    0, 1, ILD_INPUT_INSTANCE, 1);

	inputLayout = GraphicsEngine::get()->getRenderSystem()->createInputLayout(inputLayoutDesc.getLayout(), L"GrassVertexShader.hlsl", "main");
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
