#include "PerlinNoise.h"

#include <random>
#include <algorithm>

PerlinNoise::PerlinNoise(unsigned int width, unsigned int height) : width(width), height(height) {
}

PerlinNoise::~PerlinNoise() {
}

float* PerlinNoise::generateNoise1D(unsigned int length, float* output) {
	output = new float[length];
	float* result = new float[length];

	for (int i = 0; i < length; i++) {
		result[i] = (float) rand() / (float) RAND_MAX;
		output[i] = result[i];
	}

	return result;
}

float* PerlinNoise::generateNoise2D(unsigned int length, float* output) {
	unsigned int size = length * length;
	output = new float[size];
	float* result = new float[size];

	for (int i = 0; i < size; i++) {
		result[i] = (float)rand() / (float)RAND_MAX;
		output[i] = result[i];
	}

	return result;
}

float* PerlinNoise::generateNoise1D(unsigned int length, float* seed, unsigned int octaves, float* output) {
	width = length;
	height = length;

	output = new float[length];
	float* result = new float[length];

	for (int i = 0; i < length; i++) {
		float noiseValue = 0.0f;
		float scale = 1.0f;
		float scaleAccumulation = 0.0f;
		for (int j = 0; j < octaves; j++) {
			unsigned int pitch = length >> j;
			unsigned int sample1 = (i / pitch) * pitch;
			unsigned int sample2 = (sample1 + pitch) % length;

			float blend = (float) (i - sample1) / (float) pitch;
			float sample = (1.0f - blend) * seed[sample1] + blend * seed[sample2];

			noiseValue += sample * scale;
			scaleAccumulation += scale;
			scale /= 2.0f;
		}
		
		result[i] = noiseValue / scaleAccumulation;
		output[i] = result[i];
	}

	return result;
}

float* PerlinNoise::generateNoise2D(unsigned int length, float* seed, unsigned int octaves, float* output) {
	width = length;
	height = length;

	unsigned int size = length * length;

	output = new float[size];
	float* result = new float[size];

	for (int x = 0; x < length; x++) {
		for (int y = 0; y < length; y++) {

			float noiseValue = 0.0f;
			float scale = 1.0f;
			float scaleAccumulation = 0.0f;

			for (int octave = 0; octave < octaves; octave++) {
				unsigned int pitch = length >> octave;
				unsigned int sampleX1 = (x / pitch) * pitch;
				unsigned int sampleY1 = (y / pitch) * pitch;

				unsigned int sampleX2 = (sampleX1 + pitch) % length;
				unsigned int sampleY2 = (sampleY1 + pitch) % length;

				float blendX = (float) (x - sampleX1) / (float) pitch;
				float blendY = (float) (y - sampleY1) / (float) pitch;

				float sampleT = (1.0f - blendX) * seed[sampleY1 * length + sampleX1] + blendX * seed[sampleY1 * length + sampleX2];
				float sampleB = (1.0f - blendX) * seed[sampleY2 * length + sampleX1] + blendX * seed[sampleY2 * length + sampleX2];

				noiseValue += (blendY * (sampleB - sampleT) + sampleT) * scale;
				scaleAccumulation += scale;
				scale /= 2.0f;
			}

			result[y * length + x] = noiseValue / scaleAccumulation;
			output[y * length + x] = result[y * length + x];
		}
	}

	return result;
}

unsigned int PerlinNoise::getWidth() {
	return width;
}

unsigned int PerlinNoise::getHeight() {
	return height;
}
