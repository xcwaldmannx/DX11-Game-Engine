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
			};
			instances.push_back(inst);
		}
	}


	void* shaderByteCode = nullptr;
	size_t sizeShaderByteCode = 0;

	GraphicsEngine::get()->getRenderSystem()->compileVertexShader(L"GrassVertexShader.hlsl", "main", &shaderByteCode, &sizeShaderByteCode);
	vertexBuffer = GraphicsEngine::get()->getRenderSystem()->createVertexBuffer(&mesh->getVertices()[0], sizeof(GrassVertex), (UINT)mesh->getVertices().size(), VertexBuffer::INPUT_LAYOUT_STANDARD_INST, (UINT*)shaderByteCode, (UINT)sizeShaderByteCode);
	GraphicsEngine::get()->getRenderSystem()->releaseCompiledShader();

	GraphicsEngine::get()->getRenderSystem()->compileVertexShader(L"GrassVertexShader.hlsl", "main", &shaderByteCode, &sizeShaderByteCode);
	instanceBuffer = GraphicsEngine::get()->getRenderSystem()->createInstanceBuffer(&instances[0], sizeof(GrassInstance), (UINT)instances.size(), (UINT*)shaderByteCode, (UINT)sizeShaderByteCode);
	GraphicsEngine::get()->getRenderSystem()->releaseCompiledShader();

	indexBuffer = GraphicsEngine::get()->getRenderSystem()->createIndexBuffer(&mesh->getIndices()[0], mesh->getIndices().size());
}

GrassSystem::~GrassSystem() {
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
