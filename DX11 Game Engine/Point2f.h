#pragma once

class Point2f {
public:
	Point2f() : x(0), y(0) {

	}

	Point2f(float x, float y) : x(x), y(y) {

	}

	Point2f(const Point2f& point) : x(point.x), y(point.y) {

	}

	~Point2f() {

	}

	float x = 0;
	float y = 0;
};