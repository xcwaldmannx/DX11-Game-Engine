#pragma once

#include "System.h"

#include "Coordinator.h"

#include "ConstantBuffer.h"
#include "ConstantBufferTypes.h"

#include "ComponentTypes.h"

class TransformSystem : public System {
public:
    void init(long double deltaTime);
    void update(long double deltaTime);

private:
    void calculateTransformation(TransformBuffer* buffer, const Vec3f& transform);
    void calculateRotation(TransformBuffer* buffer, const Vec3f& rotate);
    void calculateScale(TransformBuffer* buffer, const Vec3f& scale);
};
