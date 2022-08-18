#pragma once

#include "Vec2f.h"
#include "Vec3f.h"

class VertexMesh {
public:
	VertexMesh() : position(), texcoord(), normal() {
	}


	VertexMesh(Vec3f position, Vec2f texcoord, Vec3f normal) : position(position), texcoord(texcoord), normal(normal) {
	}

	VertexMesh(const VertexMesh& mesh) : position(mesh.position), texcoord(mesh.texcoord), normal(mesh.normal) {
	}

	~VertexMesh() {
	}

	Vec3f position{};
	Vec2f texcoord{};
	Vec3f normal{};

	bool operator==(const VertexMesh& other) {
		return position == other.position && texcoord == other.texcoord && normal == other.normal;
	}
};