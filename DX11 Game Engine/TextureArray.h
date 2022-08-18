#pragma once

#include <d3d11.h>

#include "Prerequisites.h"

class TextureArray {
public:
	TextureArray(unsigned int maxTextures);
	~TextureArray();

	void addTexture(const TexturePtr& texture);

	ID3D11ShaderResourceView** getTextures();

	unsigned int size();

private:
	ID3D11ShaderResourceView* textures[32]{};
	unsigned int textureCount = 0;
	unsigned int maxTextures = 0;

};

