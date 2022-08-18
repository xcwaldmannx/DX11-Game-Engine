#pragma once

#include <d3d11.h>

#include "Resource.h"

class Texture : public Resource {
public:
	Texture(const wchar_t* fullPath);
	~Texture();

private:
	ID3D11Resource* texture = nullptr;
	ID3D11ShaderResourceView* shaderResourceView = nullptr;

	friend class DeviceContext;
	friend class TextureArray;
};

