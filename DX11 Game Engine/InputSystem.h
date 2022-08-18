#pragma once

#include <unordered_set>

#include "InputListener.h"
#include "Point2f.h"

class InputSystem {
private:
	InputSystem();
	~InputSystem();
public:
	void update();
	void addListener(InputListener* listener);
	void removeListener(InputListener* listener);

	void setCursorPosition(const Point2f& pos);
	void showCursor(bool show);

	static void create();
	static void release();
	static InputSystem* get();

private:
	static InputSystem* inputSystem;

	std::unordered_set<InputListener*> listeners{};
	unsigned char keysState[256] = {};
	unsigned char keysStateOld[256] = {};

	Point2f oldMousePos{};
	bool firstTime = true;

};

