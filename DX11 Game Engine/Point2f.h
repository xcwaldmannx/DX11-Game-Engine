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

	Point2f operator+ (Point2f p) {
		return Point2f(x + p.x, y + p.y);
	}

	void operator+= (Point2f p) {
		x += p.x;
		y += p.y;
	}

	Point2f operator- (Point2f p) {
		return Point2f(x - p.x, y - p.y);
	}

	void operator-= (Point2f p) {
		x -= p.x;
		y -= p.y;
	}

	float x = 0;
	float y = 0;
};