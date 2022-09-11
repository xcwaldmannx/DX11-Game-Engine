#pragma once

#include <array>
#include <list>
#include <vector>
#include <memory>
#include <iostream>

#include "LODRadius.h"

class LODSystem {
public:
	LODSystem(const size_t smallestRadius, const size_t largestRadius, unsigned int maxLevels) : maxLevels(maxLevels) {
		sizePerRadius = (largestRadius - smallestRadius) / maxLevels;
		float radius = smallestRadius + sizePerRadius;
		for (unsigned int i = 1; i <= maxLevels; i++) {
			LODRadius level = {
				{0.0f, 0.0f},
				radius,
				i,
			};
			lodLevels.push_back(level);
			radius += sizePerRadius;
		}
	}

	~LODSystem() {

	}

	void updatePosition(float x, float z) {
		for (int i = 0; i < lodLevels.size(); i++) {
			lodLevels[i].center = {x, z};
		}
	}

	const unsigned int getLODOf(const LODRadius& item) {
		unsigned int level = 1;
		for (int i = 0; i < lodLevels.size(); i++) {
			if (lodLevels[i].contains(item.center)) {
				return level;
			}
			level++;
		}
		return maxLevels;
	}

	const float sizePerLevel() {
		return sizePerRadius;
	}

	const unsigned int maxLODLevel() {
		return maxLevels;
	}

protected:
	// LOD levels list
	unsigned int maxLevels;
	std::vector<LODRadius> lodLevels;

	// data
	float sizePerRadius;
};