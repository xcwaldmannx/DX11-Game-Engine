#include "ResourceManager.h"

#include <filesystem>

ResourceManager::ResourceManager() {
}

ResourceManager::~ResourceManager() {
}

ResourcePtr ResourceManager::createResourceFromFile(const wchar_t* filepath) {
	std::wstring fullPath = std::filesystem::absolute(filepath);

	auto it = resources.find(fullPath);
	if (it != resources.end()) {
		return it->second;
	}

	Resource* raw = this->createResourceFromFileConcrete(fullPath.c_str());

	if (raw) {
		ResourcePtr res(raw);
		resources[fullPath] = res;
		return res;
	}

	return nullptr;
}
