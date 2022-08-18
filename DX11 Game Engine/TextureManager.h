#pragma once

#include "ResourceManager.h"

class TextureManager : public ResourceManager {
public:
	TextureManager();
	~TextureManager();

	TexturePtr createTextureFromFile(const wchar_t* filepath);

	TextureArrayPtr createTextureArray(unsigned int maxTextures);

protected:
	virtual Resource* createResourceFromFileConcrete(const wchar_t* filepath);
};

