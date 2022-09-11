#include "InputLayoutDescriptor.h"

InputLayoutDescriptor::InputLayoutDescriptor() {
}

InputLayoutDescriptor::~InputLayoutDescriptor() {
}

void InputLayoutDescriptor::setDefault() {
	m_layout.clear();
}

void InputLayoutDescriptor::addElement(ILD_TYPE type, int semanticIndex, int inputSlot, ILD_INPUT input, int instanceDataStepRate) {
	// Calculate this elements alginedByteOffset
	LPCSTR semanticName = m_ILDTypes[type];
	DXGI_FORMAT format = m_ILDSizes[m_ILDTypeSizes[type]];
	D3D11_INPUT_CLASSIFICATION inputSlotClass = m_ILDInputs[input];

	int alignedByteOffset = m_alignedByteOffsetsByInputSlot[inputSlot];
	m_alignedByteOffsetsByInputSlot[inputSlot] += m_ILDTypeSizes[type];

	D3D11_INPUT_ELEMENT_DESC desc = {
		semanticName,
		semanticIndex,
		format,
		inputSlot,
		alignedByteOffset,
		inputSlotClass,
		instanceDataStepRate,
	};
	m_layout.push_back(desc);
}

void InputLayoutDescriptor::addCustomElement(LPCSTR semanticName, int semanticIndex, ILD_SIZE size, int inputSlot, ILD_INPUT input, int instanceDataStepRate) {
	// Calculate this elements alginedByteOffset
	DXGI_FORMAT format = m_ILDSizes[size];
	D3D11_INPUT_CLASSIFICATION inputSlotClass = m_ILDInputs[input];

	int alignedByteOffset = m_alignedByteOffsetsByInputSlot[inputSlot];
	m_alignedByteOffsetsByInputSlot[inputSlot] += size;

	D3D11_INPUT_ELEMENT_DESC desc = {
		semanticName,
		semanticIndex,
		format,
		inputSlot,
		alignedByteOffset,
		inputSlotClass,
		instanceDataStepRate,
	};
	m_layout.push_back(desc);
}

std::vector<D3D11_INPUT_ELEMENT_DESC> InputLayoutDescriptor::getLayout() {
	return m_layout;
}
