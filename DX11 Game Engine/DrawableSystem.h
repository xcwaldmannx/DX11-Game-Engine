#pragma once

#include "System.h"

#include "Coordinator.h"

#include "ConstantBuffer.h"
#include "ConstantBufferTypes.h"

#include "ComponentTypes.h"

class DrawableSystem : public System {
public:
    void render();
};
