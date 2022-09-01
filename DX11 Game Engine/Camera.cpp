#include "Camera.h"

#include <iostream>

#include "InputSystem.h"

Camera::Camera(LODTerrain* terrain) : terrain(terrain) {
}

Camera::~Camera() {
}

void Camera::updateView(Window* window, long double deltaTime) {

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

    float speed = 5.0f * deltaTime;

    Vec3f camPosX;
    Vec3f camPosY;
    Vec3f camPosZ;
    Vec3f newCamPos;
    Vec3f oldCamPos = m_worldCamera.getTranslation();
    float terrainHeight = 0;

    if (m_flyingMode) {
        speed = 100.0f * deltaTime;
        camPosX = camera.getXDirection() * (m_right * speed);
        camPosY = camera.getYDirection() * (m_up * speed);
        camPosZ = camera.getZDirection() * (m_forward * speed);
        newCamPos = m_worldCamera.getTranslation() + camPosX + camPosY + camPosZ;
    } else {
        camPosX = Vec3f(camera.getXDirection().x, 0, camera.getXDirection().z).normalize() * (m_right * speed);
        camPosZ = Vec3f(camera.getZDirection().x, 0, camera.getZDirection().z).normalize() * (m_forward * speed);
        newCamPos = m_worldCamera.getTranslation() + camPosX + camPosZ;
        if (terrain) terrainHeight = terrain->getHeightAt(newCamPos.x, newCamPos.z);

        if (m_up == 1.0 && !m_isInAir) {
            jumpForceN = 25.0f;
            m_isInAir = true;
        }

        if (m_isInAir) {
            float forceN = m_mass * m_gravity;
            newCamPos.y += jumpForceN * deltaTime;
            jumpForceN += forceN;
            if (newCamPos.y - 1.5f <= terrainHeight) {
                newCamPos.y = 1.5f + terrainHeight;
                m_isInAir = false;
            }
        }
        else {
            newCamPos.y = 1.5f + terrainHeight;
        }
    }

    m_worldPosition = newCamPos;
    camera.setTranslation(m_worldPosition);

    this->m_worldCamera = camera;
    camera.inverse();

    m_viewCamera = camera;

    int width = window->getWindowRect().right - window->getWindowRect().left;
    int height = window->getWindowRect().bottom - window->getWindowRect().top;
    float aspect = (float)width / (float)height;

    m_projCamera.setPerspective(1.57f, aspect, 0.1f, 4096.0f);


}

void Camera::updateMovement(float forward, float right, float up, bool flyingMode) {
    m_flyingMode = flyingMode;
    m_forward = forward;
    m_right = right;
    m_up = up;

}

void Camera::updateMouse(float rotX, float rotY) {
    m_rotX = rotX;
    m_rotY = rotY;
}

const Vec3f& Camera::getWorldPosition() {
    return m_worldPosition;
}

Vec3f Camera::getMousePosition() {
    Vec3f lastPosition = m_worldCamera.getTranslation();
    Vec3f testPosition = m_worldCamera.getTranslation();

    float maxDist = 100;

    for (int i = 0; i < maxDist; i++) {
        if (testPosition.y < terrain->getHeightAt(testPosition.x, testPosition.z)) {
            Vec3f middlePosition;
            for (int j = 0; j < maxDist; j++) {
                float distBetween = Vec3f::dist(lastPosition, testPosition) / 2.0f;
                middlePosition = lastPosition + m_worldCamera.getZDirection() * distBetween;

                if (middlePosition.y < terrain->getHeightAt(middlePosition.x, middlePosition.z)) {
                    testPosition = middlePosition;
                } else {
                    lastPosition = middlePosition;
                }
            }
            return middlePosition;
        }
        lastPosition = testPosition;
        testPosition += m_worldCamera.getZDirection();
    }
    return m_worldCamera.getTranslation() + m_worldCamera.getZDirection() * maxDist;
}

Mat4f& Camera::getWorld() {
    return m_worldCamera;
}

Mat4f& Camera::getView() {
    return m_viewCamera;
}

Mat4f& Camera::getProj() {
    return m_projCamera;
}
