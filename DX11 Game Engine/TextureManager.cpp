#include "TextureManager.h"

#include "Texture.h"
#include "TextureArray.h"

TextureManager::TextureManager() : ResourceManager() {
}

TextureManager::~TextureManager() {
}

TexturePtr TextureManager::createTextureFromFile(const wchar_t* filepath) {
	return std::static_pointer_cast<Texture>(createResourceFromFile(filepath));
}

TextureArrayPtr TextureManager::createTextureArray(unsigned int maxTextures) {
	TextureArrayPtr texture = nullptr;
	try {
		texture = std::make_shared<TextureArray>(maxTextures);
	} catch (...) {}
	return texture;
}

Resource* TextureManager::createResourceFromFileConcrete(const wchar_t* filepath) {
	Texture* texture = nullptr;
	try {
		texture = new Texture(filepath);
	} catch (...) {}
	return texture;
}