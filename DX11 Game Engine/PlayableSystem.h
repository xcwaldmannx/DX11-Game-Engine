#pragma once

#include "System.h"

#include "Coordinator.h"

#include "ConstantBuffer.h"
#include "ConstantBufferTypes.h"

#include "ComponentTypes.h"

class PlayableSystem : public System {
public:
	PlayableSystem();
	~PlayableSystem();

	void update(long double deltaTime);
};