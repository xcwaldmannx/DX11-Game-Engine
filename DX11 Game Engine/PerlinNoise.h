#pragma once

class PerlinNoise {
public:
	PerlinNoise();
	~PerlinNoise();

	static float* generateSeed1D(unsigned int length, float scale, bool overlap, float* output);
	static float** generateSeed2D(unsigned int length, float scale, bool overlap, float** output);
	static float** generateNoise2D(unsigned int length, float** seed, unsigned int octaves, float** output);
	static float* generateNoise1D(unsigned int length, float* seed, unsigned int octaves, float* output);

};

