#pragma once

#include "Vec2f.h"
#include "Vec3f.h"

class Vertex {
public:
	Vertex() : position(), texcoord(), normal() {
	}


	Vertex(Vec3f position, Vec2f texcoord, Vec3f normal) : position(position), texcoord(texcoord), normal(normal) {
	}

	Vertex(const Vertex& mesh) : position(mesh.position), texcoord(mesh.texcoord), normal(mesh.normal) {
	}

	~Vertex() {
	}

	Vec3f position{};
	Vec2f texcoord{};
	Vec3f normal{};

	bool operator==(const Vertex& other) {
		return position == other.position && texcoord == other.texcoord && normal == other.normal;
	}
};