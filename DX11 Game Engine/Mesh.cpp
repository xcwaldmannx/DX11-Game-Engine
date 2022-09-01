#include "Mesh.h"

#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>

#include <Windows.h>

#include "GraphicsEngine.h"

Mesh::Mesh(const wchar_t* fullPath) : Resource(fullPath) {
	tinyobj::attrib_t attribs;
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;

	std::string warn;
	std::string err;

	int size_needed = WideCharToMultiByte(CP_UTF8, 0, &fullPath[0], (int) wcslen(fullPath), NULL, 0, NULL, NULL);
	std::string inputFile(size_needed, 0);
	WideCharToMultiByte(CP_UTF8, 0, &fullPath[0], (int) wcslen(fullPath), &inputFile[0], size_needed, NULL, NULL);
	std::string mtlFile = inputFile.substr(0, inputFile.find_last_of("\\/"));

	bool result = tinyobj::LoadObj(&attribs, &shapes, &materials, &warn, &err, inputFile.c_str(), mtlFile.c_str());

	if (!err.empty()) {
		throw std::exception("Mesh could not be created.");
	}

	if (!result) {
		throw std::exception("Mesh could not be created.");
	}

	size_t sizeVertexIndexLists = 0;
	for (size_t s = 0; s < shapes.size(); s++) {
		sizeVertexIndexLists += shapes[s].mesh.indices.size();
	}

	vertices.reserve(sizeVertexIndexLists);
	indices.reserve(sizeVertexIndexLists);

	materialSlots.resize(materials.size());

	size_t indexGlobalOffset = 0;

	for (size_t m = 0; m < materials.size(); m++) {
		materialSlots[m].startIndex = indexGlobalOffset;
		materialSlots[m].materialID = m;

		for (size_t s = 0; s < shapes.size(); s++) {
			size_t indexOffset = 0;

			for (size_t f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++) {
				if (shapes[s].mesh.material_ids[f] != m) {
					continue;
				}

				unsigned char numFaceVertices = shapes[s].mesh.num_face_vertices[f];

				for (unsigned char v = 0; v < numFaceVertices; v++) {
					tinyobj::index_t index = shapes[s].mesh.indices[indexOffset + v];

					tinyobj::real_t vx = attribs.vertices[index.vertex_index * 3 + 0];
					tinyobj::real_t vy = attribs.vertices[index.vertex_index * 3 + 1];
					tinyobj::real_t vz = attribs.vertices[index.vertex_index * 3 + 2];

					tinyobj::real_t tx = attribs.texcoords[index.texcoord_index * 2 + 0];
					tinyobj::real_t ty = attribs.texcoords[index.texcoord_index * 2 + 1];

					tinyobj::real_t nx = attribs.normals[index.normal_index * 3 + 0];
					tinyobj::real_t ny = attribs.normals[index.normal_index * 3 + 1];
					tinyobj::real_t nz = attribs.normals[index.normal_index * 3 + 2];

					Vec3f position = Vec3f(vx, vy, vz);
					Vec2f texcoord = Vec2f(tx, ty);
					Vec3f normal = Vec3f(nx, ny, nz);
					logAxialMinAndMax(position);

					Vertex vertex(position, texcoord, normal);
					vertices.push_back(vertex);
					indices.push_back((unsigned int)(indexGlobalOffset + v));

				}
				indexOffset += numFaceVertices;
				indexGlobalOffset += numFaceVertices;
			}
		}

		materialSlots[m].numIndices = indexGlobalOffset - materialSlots[m].startIndex;
	}


	void* shaderByteCode = nullptr;
	size_t sizeShaderByteCode = 0;

	GraphicsEngine::get()->getVertexLayoutShaderData(&shaderByteCode, &sizeShaderByteCode);
	vertexBuffer = GraphicsEngine::get()->getRenderSystem()->createVertexBuffer(&vertices[0], sizeof(Vertex), (UINT) vertices.size());

	indexBuffer = GraphicsEngine::get()->getRenderSystem()->createIndexBuffer(&indices[0], (UINT) indices.size());

	// bounding box
	generateAABB();
}

Mesh::~Mesh() {
}

const VertexBufferPtr& Mesh::getVertexBuffer() {
	return vertexBuffer;
}

const IndexBufferPtr& Mesh::getIndexBuffer() {
	return indexBuffer;
}

const MaterialSlot Mesh::getMaterialSlot(unsigned int slot) {
	if (slot >= materialSlots.size()) {
		return MaterialSlot();
	}

	return materialSlots[slot];
}

size_t Mesh::materialSlotCount() {
	return materialSlots.size();
}

std::vector<Vertex> Mesh::getVertices() {
	return vertices;
}

std::vector<unsigned int> Mesh::getIndices() {
	return indices;
}

const VertexBufferPtr& Mesh::getVertexBufferBB() {
	return vertexBufferBB;
}

const IndexBufferPtr& Mesh::getIndexBufferBB() {
	return indexBufferBB;
}

void Mesh::logAxialMinAndMax(const Vec3f& vec) {
	axialMin.x = vec.x < axialMin.x ? vec.x : axialMin.x;
	axialMin.y = vec.y < axialMin.y ? vec.y : axialMin.y;
	axialMin.z = vec.z < axialMin.z ? vec.z : axialMin.z;

	axialMax.x = vec.x > axialMax.x ? vec.x : axialMax.x;
	axialMax.y = vec.y > axialMax.y ? vec.y : axialMax.y;
	axialMax.z = vec.z > axialMax.z ? vec.z : axialMax.z;
}

void Mesh::generateAABB() {
	 std::vector<Vec3f>verticesBB = {
		// FRONT FACE VERTICES
		// top left
		Vec3f(axialMin.x, axialMax.y, axialMin.z),
		// bottom left
		Vec3f(axialMin.x, axialMin.y, axialMin.z),
		// bottom right
		Vec3f(axialMax.x, axialMin.y, axialMin.z),
		// top right
		Vec3f(axialMax.x, axialMax.y, axialMin.z),

		// BACK FACE VERTICES
		// top left
		Vec3f(axialMin.x, axialMax.y, axialMax.z),
		// bottom left
		Vec3f(axialMin.x, axialMin.y, axialMax.z),
		// bottom right
		Vec3f(axialMax.x, axialMin.y, axialMax.z),
		// top right
		Vec3f(axialMax.x, axialMax.y, axialMax.z)
	};

	std::vector<unsigned int>indicesBB = {
		// FRONT
		0, 1,
		1, 2,
		2, 3,
		3, 0,

		// BACK
		4, 5,
		5, 6,
		6, 7,
		7, 4,

		// LEFT
		0, 4,
		4, 5,
		5, 1,
		1, 0,

		// RIGHT
		3, 7,
		7, 6,
		6, 2,
		2, 3,

		// TOP
		0, 3,
		3, 7,
		7, 4,
		4, 0,

		// BOTTOM
		1, 2,
		2, 6,
		6, 5,
		5, 1,
	};

	void* shaderByteCode = nullptr;
	size_t sizeShaderByteCode = 0;

	vertexBufferBB = GraphicsEngine::get()->getRenderSystem()->createVertexBuffer(&verticesBB[0], sizeof(Vec3f), (UINT)verticesBB.size());

	indexBufferBB = GraphicsEngine::get()->getRenderSystem()->createIndexBuffer(&indicesBB[0], (UINT)indicesBB.size());
}
