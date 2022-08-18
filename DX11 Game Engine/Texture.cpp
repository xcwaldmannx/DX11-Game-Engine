#include "Texture.h"

#include <iostream>

#include "DirectXTex.h"
#include "GraphicsEngine.h"

Texture::Texture(const wchar_t* fullPath) : Resource(fullPath) {
	DirectX::ScratchImage imgData;
	HRESULT result = DirectX::LoadFromWICFile(fullPath, DirectX::WIC_FLAGS_IGNORE_SRGB, nullptr, imgData);

	if (SUCCEEDED(result)) {
		result = DirectX::CreateTexture(GraphicsEngine::get()->getRenderSystem()->d3dDevice,
			imgData.GetImages(), imgData.GetImageCount(), imgData.GetMetadata(), &texture);

		
		D3D11_TEXTURE2D_DESC texDesc = {};
		texDesc.Width = (UINT) imgData.GetMetadata().width;
		texDesc.Height = (UINT) imgData.GetMetadata().height;
		texDesc.Format = imgData.GetMetadata().format;
		texDesc.Usage = D3D11_USAGE_DEFAULT;
		texDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
		texDesc.MipLevels = 0;
		texDesc.SampleDesc.Count = 1;
		texDesc.SampleDesc.Quality = 0;
		texDesc.MiscFlags = D3D11_RESOURCE_MISC_GENERATE_MIPS;
		texDesc.ArraySize = 1;
		texDesc.CPUAccessFlags = 0;

		ID3D11Texture2D* texture2D = nullptr;
		GraphicsEngine::get()->getRenderSystem()->d3dDevice->CreateTexture2D(&texDesc, nullptr, &texture2D);
		if (texture2D) {
			GraphicsEngine::get()->getRenderSystem()->d3dDeviceContext->UpdateSubresource(texture2D, 0u, nullptr, imgData.GetPixels(), (UINT) imgData.GetMetadata().width * sizeof(unsigned char) * 4, (UINT) imgData.GetMetadata().depth);
		}
		

		D3D11_SHADER_RESOURCE_VIEW_DESC desc = {};
		desc.Format = imgData.GetMetadata().format;
		desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		desc.Texture2D.MipLevels = -1;
		desc.Texture2D.MostDetailedMip = 0;

		GraphicsEngine::get()->getRenderSystem()->d3dDevice->CreateShaderResourceView(texture2D, &desc, &shaderResourceView);

		if (shaderResourceView) {
			GraphicsEngine::get()->getRenderSystem()->d3dDeviceContext->GenerateMips(shaderResourceView);
		} else {
			std::cout << "Mipmaps could not be generated." << std::endl;
		}
	} else {
		throw std::exception("Texture could not be created.");
	}
}

Texture::~Texture() {
	shaderResourceView->Release();
	texture->Release();
}
