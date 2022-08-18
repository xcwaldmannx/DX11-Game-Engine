#include "TransformSystem.h"

#include "Coordinator.h"

#include "ConstantBuffer.h"
#include "ConstantBufferTypes.h"

extern Coordinator coordinator;

float rate = 0;

void TransformSystem::init(long double deltaTime) {
    for (auto const& entity : entities) {
        auto& transform = coordinator.getComponent<TransformComponent>(entity);
        auto& drawable = coordinator.getComponent<DrawableComponent>(entity);

        calculateLocation(transform, drawable);
    }
}

void TransformSystem::update(long double deltaTime) {
    for (auto const& entity : entities) {
        auto& transform = coordinator.getComponent<TransformComponent>(entity);
        auto& drawable = coordinator.getComponent<DrawableComponent>(entity);

        calculateLocation(transform, drawable);
    }
}

void TransformSystem::calculateLocation(TransformComponent& transform, DrawableComponent& drawable) {
    TransformBuffer* transformBuffer = new TransformBuffer{};

    // TRANSFORM BUFFER
    transformBuffer->world.setIdentity();

    // scale
    transformBuffer->world.setScale(transform.Scale);

    // rotate
    Mat4f temp;

    // rotate self
    temp.setIdentity();
    temp.setRotationX(transform.Rotation.x);
    transformBuffer->world *= temp;
    temp.setIdentity();
    temp.setRotationY(transform.Rotation.y);
    transformBuffer->world *= temp;
    temp.setIdentity();
    temp.setRotationZ(transform.Rotation.z);
    transformBuffer->world *= temp;

    // rotate camera
    temp.setIdentity();
    temp.setRotationX(transform.Camera->getWorld().getRotation().x * transform.RotateWithCameraXYZ.x);
    transformBuffer->world *= temp;
    temp.setIdentity();
    temp.setRotationY(transform.Camera->getWorld().getRotation().y * transform.RotateWithCameraXYZ.y);
    transformBuffer->world *= temp;
    temp.setIdentity();
    temp.setRotationZ(transform.Camera->getWorld().getRotation().z * transform.RotateWithCameraXYZ.z);
    transformBuffer->world *= temp;

    // translate
    if (transform.TranslateWithCameraXYZ.magnitude() > 0) {
        Vec3f x = transform.Camera->getWorld().getXDirection() * transform.Position.x * transform.TranslateWithCameraXYZ.x;
        Vec3f y = transform.Camera->getWorld().getYDirection() * transform.Position.y * transform.TranslateWithCameraXYZ.y;
        Vec3f z = transform.Camera->getWorld().getZDirection() * transform.Position.z * transform.TranslateWithCameraXYZ.z;
        Vec3f position = transform.Camera->getWorld().getTranslation() + x + y + z;
        transformBuffer->world.setTranslation(position);
    } else {
        transformBuffer->world.setTranslation(transform.Position);
    }

    // view and projection
    transformBuffer->view = transform.Camera->getView();
    transformBuffer->proj = transform.Camera->getProj();

    // update constant buffers
    drawable.updateBuffer<TransformBuffer*>(TRANSFORM, transformBuffer);
    transformBuffer = nullptr;
}
