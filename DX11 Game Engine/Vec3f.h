#pragma once

#include <cmath>

class Vec3f {
public:
	Vec3f(): x(0), y(0), z(0) {
	}

	Vec3f(float x, float y, float z) : x(x), y(y), z(z) {
	}

	Vec3f(const Vec3f &vector) : x(vector.x), y(vector.y), z(vector.z) {
	}

	~Vec3f() {

	}

	static Vec3f lerp(const Vec3f& start, const Vec3f& end, float delta) {
		Vec3f result;
		result.x = start.x * (1.0f - delta) + end.x * (delta);
		result.y = start.y * (1.0f - delta) + end.y * (delta);
		result.z = start.z * (1.0f - delta) + end.z * (delta);
		return result;
	}

	Vec3f normalize() {
		Vec3f result;
		float value = (float) sqrt(x * x + y * y + z * z);
		result.x = x / value;
		result.y = y / value;
		result.z = z / value;
		return result;
	}

	float magnitude() {
		return (float) sqrt(x * x + y * y + z * z);
	}

	Vec3f operator+ (Vec3f vec) {
		return Vec3f(x + vec.x, y + vec.y, z + vec.z);
	}

	void operator+= (Vec3f vec) {
		x += vec.x;
		y += vec.y;
		z += vec.z;
	}

	Vec3f operator- (Vec3f vec) {
		return Vec3f(x - vec.x, y - vec.y, z - vec.z);
	}

	void operator-= (Vec3f vec) {
		x -= vec.x;
		y -= vec.y;
		z -= vec.z;
	}

	Vec3f operator* (float num) {
		return Vec3f(x * num, y * num, z * num);
	}

	void operator*= (Vec3f vec) {
		x *= vec.x;
		y *= vec.y;
		z *= vec.z;
	}

	bool operator== (Vec3f vec) {
		return x == vec.x && y == vec.y && z == vec.z;
	}

	float x = 0;
	float y = 0;
	float z = 0;
};