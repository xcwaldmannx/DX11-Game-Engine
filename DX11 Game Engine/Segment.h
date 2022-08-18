#pragma once

#include "Point2f.h"

class Segment {
public:
	Segment() : p1(0, 0), p2(0, 0) {
	}

	Segment(int x1, int y1, int x2, int y2) : p1(x1, y1), p2(x2, y2) {
	}

	Segment(const Point2f& p1, const Point2f& p2) : p1(p1), p2(p2) {
	}

	~Segment() {

	}

	bool intersects(const Segment& segment1, const Segment& segment2) {

		const Point2f a = segment1.p1;
		const Point2f b = segment1.p2;
		const Point2f c = segment2.p1;
		const Point2f d = segment2.p2;

		float determinant1 = (a.x - c.x) * (b.y - c.y) - (a.y - c.y) * (b.x - c.x);
		float determinant2 = (a.x - c.x) * (b.y - c.y) - (a.y - c.y) * (b.x - c.x);

		// incomplete

		return false;
	}

public:
	Point2f p1{};
	Point2f p2{};

};