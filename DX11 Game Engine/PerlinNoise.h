#pragma once

class PerlinNoise {
public:
	PerlinNoise(unsigned int width, unsigned int height);
	~PerlinNoise();

	float* generateNoise1D(unsigned int length, float* output);
	float* generateNoise2D(unsigned int length, float* output);
	float* generateNoise1D(unsigned int length, float* seed, unsigned int octaves, float* output);
	float* generateNoise2D(unsigned int length, float* seed, unsigned int octaves, float* output);

	unsigned int getWidth();
	unsigned int getHeight();

private:
	unsigned int width = 0;
	unsigned int height = 0;

};

