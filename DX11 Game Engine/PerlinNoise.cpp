#include "PerlinNoise.h"

#include <random>
#include <algorithm>

PerlinNoise::PerlinNoise() {
}

PerlinNoise::~PerlinNoise() {
}

float* PerlinNoise::generateSeed1D(unsigned int length, bool overlap, float* output) {
	output = new float[length];
	float* result = new float[length];

	int start = 0;
	int end = length;

	if (overlap && length > 1) {
		start = 1;
		end = length - 1;
		result[start] = 0.5f;
		result[end] = 0.5f;
		output[start] = 0.5f;
		output[end] = 0.5f;
	}

	for (int i = start; i < end; i++) {
		result[i] = (float) rand() / (float) RAND_MAX;
		output[i] = result[i];
	}

	return result;
}

float** PerlinNoise::generateSeed2D(unsigned int length, bool overlap, float** output) {
	unsigned int size = length * length;

	output = new float*[length];
	float** result = new float*[size];

	for (int i = 0; i < length; i++) {
		output[i] = new float[length];
		result[i] = new float[length];
	}

	int start = 0;
	int end = length;

	if (overlap && length > 1) {
		start = 1;
		end = length - 1;
		for (int i = 0; i < length; i++) {
			result[i][0] = 0.5f;
			result[0][i] = 0.5f;
			result[length - 1][i] = 0.5f;
			result[i][length - 1] = 0.5f;

			output[i][0] = 0.5f;
			output[0][i] = 0.5f;
			output[length - 1][i] = 0.5f;
			output[i][length - 1] = 0.5f;
		}
	}

	for (int i = start; i < end; i++) {
		for (int j = start; j < end; j++) {
			result[i][j] = (float)rand() / (float)RAND_MAX;
			output[i][j] = result[i][j];
		}
	}

	return result;
}

float* PerlinNoise::generateNoise1D(unsigned int length, float* seed, unsigned int octaves, float* output) {
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

float** PerlinNoise::generateNoise2D(unsigned int length, float** seed, unsigned int octaves, float** output) {
	unsigned int size = length * length;

	// allocate space
	output = new float*[length];
	float** result = new float*[length];

	for (int i = 0; i < length; i++) {
		output[i] = new float[length];
		result[i] = new float[length];
	}

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

				float sampleT = (1.0f - blendX) * seed[sampleX1][sampleY1] + blendX * seed[sampleX2][sampleY1];
				float sampleB = (1.0f - blendX) * seed[sampleX1][sampleY2] + blendX * seed[sampleX2][sampleY2];

				noiseValue += (blendY * (sampleB - sampleT) + sampleT) * scale;
				scaleAccumulation += scale;
				scale /= 2.0f;
			}

			if (x == 0 || x == length - 1 || y == 0 || y == length - 1) {
				result[x][y] = seed[x][y];
				output[x][y] = result[x][y];
				continue;
			}

			result[x][y] = noiseValue / scaleAccumulation;
			output[x][y] = result[x][y];
		}
	}

	return result;
}
