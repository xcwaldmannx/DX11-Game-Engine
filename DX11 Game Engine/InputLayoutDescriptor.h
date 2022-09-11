#pragma once

#include <d3d11.h>
#include <vector>
#include <unordered_map>

// semantic names
typedef enum ILD_TYPE {
	ILD_TYPE_POSITION = 0,
	ILD_TYPE_TEXCOORD = 1,
	ILD_TYPE_NORMAL   = 2,
	ILD_TYPE_COLOR    = 3,
};

// formats
typedef enum ILD_SIZE {
	ILD_SIZE_F1 = 4,
	ILD_SIZE_F2 = 8,
	ILD_SIZE_F3 = 12,
	ILD_SIZE_F4 = 16
};

// input slot class
typedef enum ILD_INPUT {
	ILD_INPUT_VERTEX = 0,
	ILD_INPUT_INSTANCE = 1,
};

struct LayoutElement {
	const char* semanticName = "";
	unsigned int semanticIndex = 0;
	DXGI_FORMAT format = DXGI_FORMAT_R32G32B32_FLOAT;
	unsigned int inputSlot = 0;
	unsigned int alignedByteOffset = 0;
	D3D11_INPUT_CLASSIFICATION inputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	unsigned int instanceDataStepRate = 0;
};

class InputLayoutDescriptor {
public:
	InputLayoutDescriptor();
	~InputLayoutDescriptor();

public:
	void setDefault();
	void addElement(ILD_TYPE type, int semanticIndex, int inputSlot, ILD_INPUT input, int instanceDataStepRate);
	void addCustomElement(LPCSTR semanticName, int semanticIndex, ILD_SIZE size, int inputSlot, ILD_INPUT input, int instanceDataStepRate);
	std::vector<D3D11_INPUT_ELEMENT_DESC> getLayout();

private:
	std::vector<D3D11_INPUT_ELEMENT_DESC> m_layout;

private:
	std::unordered_map<ILD_TYPE, LPCSTR> m_ILDTypes = {
		std::pair<ILD_TYPE, LPCSTR>(ILD_TYPE_POSITION, "POSITION"),
		std::pair<ILD_TYPE, LPCSTR>(ILD_TYPE_TEXCOORD, "TEXCOORD"),
		std::pair<ILD_TYPE, LPCSTR>(ILD_TYPE_NORMAL, "NORMAL"),
		std::pair<ILD_TYPE, LPCSTR>(ILD_TYPE_COLOR, "COLOR"),
	};

	std::unordered_map<ILD_TYPE, ILD_SIZE> m_ILDTypeSizes = {
		std::pair<ILD_TYPE, ILD_SIZE>(ILD_TYPE_POSITION, ILD_SIZE_F3),
		std::pair<ILD_TYPE, ILD_SIZE>(ILD_TYPE_TEXCOORD, ILD_SIZE_F2),
		std::pair<ILD_TYPE, ILD_SIZE>(ILD_TYPE_NORMAL, ILD_SIZE_F3),
		std::pair<ILD_TYPE, ILD_SIZE>(ILD_TYPE_COLOR, ILD_SIZE_F3),
	};

	std::unordered_map<ILD_SIZE, DXGI_FORMAT> m_ILDSizes = {
		std::pair<ILD_SIZE, DXGI_FORMAT>(ILD_SIZE_F1, DXGI_FORMAT_R32_FLOAT),
		std::pair<ILD_SIZE, DXGI_FORMAT>(ILD_SIZE_F2, DXGI_FORMAT_R32G32_FLOAT),
		std::pair<ILD_SIZE, DXGI_FORMAT>(ILD_SIZE_F3, DXGI_FORMAT_R32G32B32_FLOAT),
		std::pair<ILD_SIZE, DXGI_FORMAT>(ILD_SIZE_F4, DXGI_FORMAT_R32G32B32A32_FLOAT),
	};

	std::unordered_map<ILD_INPUT, D3D11_INPUT_CLASSIFICATION> m_ILDInputs = {
		std::pair<ILD_INPUT, D3D11_INPUT_CLASSIFICATION>(ILD_INPUT_VERTEX, D3D11_INPUT_PER_VERTEX_DATA),
		std::pair<ILD_INPUT, D3D11_INPUT_CLASSIFICATION>(ILD_INPUT_INSTANCE, D3D11_INPUT_PER_INSTANCE_DATA),
	};

	std::vector<int> m_alignedByteOffsetsByInputSlot = {
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
	};
};

