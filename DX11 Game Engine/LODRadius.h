#pragma once

#include <cmath>

#include "QTRect.h"
#include "Point2f.h"

struct LODRadius {
	Point2f center;
	float radius;
	unsigned int lod;

	LODRadius(const Point2f& center, const float radius, const unsigned int lod) : center(center), radius(radius), lod(lod) {

	}

	~LODRadius() {

	}

	bool contains(const Point2f& point) const {
		float dist = sqrt((point.x - center.x) * (point.x - center.x) + (point.y - center.y) * (point.y - center.y));
		return dist < radius;
	}

	bool contains(const LODRadius& sphere) const {
		float dist = sqrt((sphere.center.x - center.x) * (sphere.center.x - center.x) + (sphere.center.y - center.y) * (sphere.center.y - center.y));
		if (dist > radius) {
			return false;
		}
		float remainder = radius - dist;
		if (sphere.radius > remainder) {
			return false;
		}
		return true;
	}
};