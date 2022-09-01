#pragma once

#include <string>
#include <vector>

#include "Vertex.h"

class Resource {
public:
	Resource(const wchar_t* fullPath);
	virtual ~Resource();
protected:
	std::wstring fullPath{};
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
};

