#pragma once

#include "System.h"

#include "Coordinator.h"

#include "ConstantBuffer.h"
#include "ConstantBufferTypes.h"

#include "ComponentTypes.h"

class PickableSystem :public System {
public:
	void update();
};

