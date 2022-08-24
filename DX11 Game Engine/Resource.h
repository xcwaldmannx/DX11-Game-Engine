#pragma once

#include <string>
#include <vector>

#include "VertexMesh.h"

class Resource {
public:
	Resource(const wchar_t* fullPath);
	virtual ~Resource();
protected:
	std::wstring fullPath{};
	std::vector<VertexMesh> vertices;
	std::vector<unsigned int> indices;
};

