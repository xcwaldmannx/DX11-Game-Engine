#pragma once

#include "Prerequisites.h"
#include "Mat4f.h"
#include "Window.h"
#include "TerrainMesh.h"

class Camera {
public:
	Camera();
	~Camera();

	void updateView(Window* window, TerrainMesh tm, long double deltaTime);
	void updateMovement(float forward, float right, float up);
	void updateMouse(float rotX, float rotY);
	const Vec3f& getWorldPosition();
	Vec3f getMousePosition();

	Mat4f getWorld();
	Mat4f getView();
	Mat4f getProj();

private:
	Window* m_window = nullptr;

	// view
	Mat4f m_worldCamera{};
	Mat4f m_viewCamera{};
	Mat4f m_projCamera{};

	// position
	Vec3f worldPosition;

	// time
	long double m_deltaTime = 0.0;

	// looking
	float m_rotX = 0.0f;
	float m_rotY = 0.0f;

	// walking/flying
	float m_forward = 0.0f;
	float m_right = 0.0f;
	float m_up = 0.0f;

	// jumping
	bool m_isInAir = false;
	float m_gravity = 0.0f;
	float m_weight = 0.01f;

	// mouse picking
	Vec3f mouseOrigin{};
	Vec3f mouseDirection{};

};

