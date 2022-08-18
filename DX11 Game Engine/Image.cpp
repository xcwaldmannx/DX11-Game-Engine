#include "Image.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

Image::Image() {
}

Image::~Image() {
}

void Image::loadImage(const std::string& filename) {
	unsigned char* raw = stbi_load(filename.c_str(), &width, &height, &componentCount, 0);
	if (raw != nullptr) {
		data = std::vector<unsigned char>(raw, raw + width * height * componentCount);
	}
	stbi_image_free(raw);

}

unsigned char Image::get(int x, int y) {
	if (x * y < data.size()) {
		return data[x + y * width];
	}
	return -1;
}

Vec4f Image::getRGBA(int x, int y) {
	if (componentCount == 3) {
		int offset = (x + y * width) * componentCount;
		float r = data[offset + 0];
		float g = data[offset + 1];
		float b = data[offset + 2];
		return Vec4f(r, g, b, 0.0f);
	} else {
		int offset = (x + y * width) * componentCount;
		float r = data[offset + 0];
		float g = data[offset + 1];
		float b = data[offset + 2];
		float a = data[offset + 3];
		return Vec4f(r, g, b, a);
	}

}

int Image::getWidth() {
	return width;
}

int Image::getHeight() {
	return height;
}

int Image::getComponentCount() {
	return componentCount;
}