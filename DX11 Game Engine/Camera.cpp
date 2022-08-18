#include "Camera.h"

#include <iostream>

#include "InputSystem.h"

Camera::Camera() {
}

Camera::~Camera() {
}

void Camera::updateView(Window* window, TerrainMesh tm, long double deltaTime) {

    m_window = window;
    m_deltaTime = deltaTime;

    // CAMERA
    Mat4f camera, temp;
    camera.setIdentity();

    temp.setIdentity();
    temp.setRotationX(m_rotX);
    camera *= temp;
    temp.setIdentity();
    temp.setRotationY(m_rotY);
    camera *= temp;

    float speed = 0.1f;

    if (m_up == -1) {
        speed = 0.2f;
    }

    Vec3f camPosX = Vec3f(camera.getXDirection().x, 0, camera.getXDirection().z).normalize() * (m_right * speed);
    Vec3f camPosZ = Vec3f(camera.getZDirection().x, 0, camera.getZDirection().z).normalize() * (m_forward * speed);
    Vec3f newCamPos = m_worldCamera.getTranslation() + camPosX + camPosZ;
    float terrainHeight = tm.getHeightAt(newCamPos.x, newCamPos.z);

    if (m_up == 1.0 && !m_isInAir) {
        m_gravity = 0.5f;
        newCamPos.y += m_gravity;
        m_isInAir = true;
    }

    if (m_isInAir) {
        m_gravity -= m_weight;
        newCamPos.y += m_gravity;
        if (newCamPos.y - 1.5f <= terrainHeight) {
            m_gravity = 0;
            newCamPos.y = 1.5f + terrainHeight;
            m_isInAir = false;
        }
    } else {
        newCamPos.y = 1.5f + terrainHeight;
    }

    worldPosition = newCamPos;
    camera.setTranslation(worldPosition);
    // MOUSE PICKING
    mouseOrigin = camera.getTranslation();
    mouseDirection = camera.getZDirection().normalize();

    this->m_worldCamera = camera;
    camera.inverse();

    m_viewCamera = camera;

    int width = window->getWindowRect().right - window->getWindowRect().left;
    int height = window->getWindowRect().bottom - window->getWindowRect().top;
    float aspect = (float)width / (float)height;

    m_projCamera.setPerspective(1.57f, aspect, 0.1f, 512.0f);


}

void Camera::updateMovement(float forward, float right, float up) {
    m_forward = forward;
    m_right = right;
    m_up = up;
}

void Camera::updateMouse(float rotX, float rotY) {
    m_rotX = rotX;
    m_rotY = rotY;
}

const Vec3f& Camera::getWorldPosition() {
    return worldPosition;
}

Vec3f Camera::getMousePosition() {
    return mouseOrigin + mouseDirection * 3.0f;
}

Mat4f Camera::getWorld() {
    return m_worldCamera;
}

Mat4f Camera::getView() {
    return m_viewCamera;
}

Mat4f Camera::getProj() {
    return m_projCamera;
}