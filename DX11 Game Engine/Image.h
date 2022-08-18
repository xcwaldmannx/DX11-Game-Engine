#pragma once

#include <string>
#include <vector>

#include "Vec4f.h"

class Image {
public:
	Image();
	~Image();

	void loadImage(const std::string& filename);

	unsigned char get(int x, int y);
	Vec4f getRGBA(int x, int y);

	int getWidth();
	int getHeight();
	int getComponentCount();

private:
	int width = 0;
	int height = 0;
	int componentCount = 0;
	std::vector<unsigned char> data{};
};

