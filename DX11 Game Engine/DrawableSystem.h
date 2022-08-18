#pragma once

#include <vector>

#include "Coordinator.h"
#include "System.h"

class DrawableSystem : public System {
public:
    void render();
};
