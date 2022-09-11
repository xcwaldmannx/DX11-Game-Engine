#pragma once

#include "Mat4f.h"
#include "Window.h"
#include "LODTerrain.h"

class LODTerrain;

class Camera {
public:
	Camera(LODTerrain* terrain, float viewDistance);
	~Camera();

	void updateView(Window* window, long double deltaTime);
	void updateMovement(float forward, float right, float up, bool flyingMode);
	void updateMouse(float rotX, float rotY);
	const Vec3f& getWorldPosition();
	Vec3f getMousePosition();

	Mat4f& getWorld();
	Mat4f& getView();
	Mat4f& getProj();

private:
	Window* m_window = nullptr;

	// terrain
	LODTerrain* terrain;
	float viewDistance;

	// view
	Mat4f m_worldCamera{};
	Mat4f m_viewCamera{};
	Mat4f m_projCamera{};

	// position
	Vec3f m_worldPosition;

	// time
	long double m_deltaTime = 0.0;

	// looking
	float m_rotX = 0.0f;
	float m_rotY = 0.0f;

	// walking/flying
	bool m_flyingMode = false;
	float m_forward = 0.0f;
	float m_right = 0.0f;
	float m_up = 0.0f;

	// jumping
	bool m_isInAir = false;
	float m_gravity = -4.9f;
	float m_mass = 1.0f;
	float jumpForceN = 25.0f;

	// mouse picking
	Vec3f mouseOrigin{};
	Vec3f mouseDirection{};

};

