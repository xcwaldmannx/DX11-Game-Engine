#include "TransformSystem.h"

extern Coordinator coordinator;

float rate = 0;

void TransformSystem::init(long double deltaTime) {
}

void TransformSystem::update(long double deltaTime) {
    for (auto const& entity : entities) {
        auto& transform = coordinator.getComponent<TransformComponent>(entity);
        auto& drawable = coordinator.getComponent<DrawableComponent>(entity);

        TransformBuffer* transformBuffer = new TransformBuffer{};

        // TRANSFORM BUFFER
        transformBuffer->world.setIdentity();

        // scale
        calculateScale(transformBuffer, transform.scale);

        // rotate from origin
        calculateRotation(transformBuffer, transform.rotation);

        if (transform.attachedToCamera) {
            // rotate with camera
            float rotX = transform.camera->getWorld().getRotation().x * transform.rotateWithCameraXYZ.x;
            float rotY = transform.camera->getWorld().getRotation().y * transform.rotateWithCameraXYZ.y;
            float rotZ = transform.camera->getWorld().getRotation().z * transform.rotateWithCameraXYZ.z;
            calculateRotation(transformBuffer, Vec3f(rotX, rotY, rotZ));

            // translate with camera
            Vec3f x = transform.camera->getWorld().getXDirection() * transform.position.x * transform.translateWithCameraXYZ.x;
            Vec3f y = transform.camera->getWorld().getYDirection() * transform.position.y * transform.translateWithCameraXYZ.y;
            Vec3f z = transform.camera->getWorld().getZDirection() * transform.position.z * transform.translateWithCameraXYZ.z;
            Vec3f position = transform.camera->getWorld().getTranslation() + x + y + z;
            calculateTransformation(transformBuffer, position);
        } else {
            // translate from origin
            calculateTransformation(transformBuffer, transform.position);
        }

        // view and projection
        transformBuffer->view = transform.camera->getView();
        transformBuffer->proj = transform.camera->getProj();

        // update constant buffers
        drawable.updateBuffer<TransformBuffer*>(TRANSFORM, transformBuffer);
        transformBuffer = nullptr;
    }
}

void TransformSystem::calculateTransformation(TransformBuffer* buffer, const Vec3f& transform) {
    buffer->world.setTranslation(transform);
}
void TransformSystem::calculateRotation(TransformBuffer* buffer, const Vec3f& rotate) {
    Mat4f tempRot;
    tempRot.setIdentity();
    tempRot.setRotationX(rotate.x);
    buffer->world *= tempRot;
    tempRot.setIdentity();
    tempRot.setRotationY(rotate.y);
    buffer->world *= tempRot;
    tempRot.setIdentity();
    tempRot.setRotationZ(rotate.z);
    buffer->world *= tempRot;
}
void TransformSystem::calculateScale(TransformBuffer* buffer, const Vec3f& scale) {
    buffer->world.setScale(scale);
}
