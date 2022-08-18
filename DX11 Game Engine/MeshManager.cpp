#include "MeshManager.h"

#include "Mesh.h"

MeshManager::MeshManager() {
}

MeshManager::~MeshManager() {
}

MeshPtr MeshManager::createMeshFromFile(const wchar_t* filepath) {
	return std::static_pointer_cast<Mesh>(createResourceFromFile(filepath));
}

Resource* MeshManager::createResourceFromFileConcrete(const wchar_t* filepath) {
	Mesh* mesh = nullptr;
	try {
		mesh = new Mesh(filepath);
	}
	catch (...) {}
	return mesh;
}