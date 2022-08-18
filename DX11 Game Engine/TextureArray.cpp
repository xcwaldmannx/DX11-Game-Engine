#include "TextureArray.h"

#include "Texture.h"

TextureArray::TextureArray(unsigned int maxTextures) : maxTextures(maxTextures) {
}

TextureArray::~TextureArray() {
}

void TextureArray::addTexture(const TexturePtr& texture) {
	if (textureCount < maxTextures) {
		textures[textureCount] = texture->shaderResourceView;
		textureCount++;
	}
}

ID3D11ShaderResourceView** TextureArray::getTextures() {
	return textures;
}

unsigned int TextureArray::size() {
	return textureCount;
}
