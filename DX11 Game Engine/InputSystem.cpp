#include "InputSystem.h"

#include <Windows.h>
#include <exception>

#include "Point2f.h"

InputSystem* InputSystem::inputSystem = nullptr;

InputSystem::InputSystem() {
}

InputSystem::~InputSystem() {
	inputSystem = nullptr;
}

void InputSystem::update() {

	POINT currentMousePos = {};
	GetCursorPos(&currentMousePos);

	if (firstTime) {
		oldMousePos = Point2f((float) currentMousePos.x, (float) currentMousePos.y);
		firstTime = false;
	}

	if (currentMousePos.x != oldMousePos.x || currentMousePos.y != oldMousePos.y) {
		// Mouse event occurred
		std::unordered_set<InputListener*>::iterator it = listeners.begin();
		while (it != listeners.end()) {
			(*it)->onMouseMove(Point2f((float) currentMousePos.x, (float) currentMousePos.y));
			it++;
		}
	}
	oldMousePos = Point2f((float) currentMousePos.x, (float) currentMousePos.y);

	if (GetKeyboardState(keysState)) {
		for (unsigned int i = 0; i < 256; i++) {
			// Key is down
			if (keysState[i] & 0x80) {
				std::unordered_set<InputListener*>::iterator it = listeners.begin();
				while (it != listeners.end()) {

					// LEFT MOUSE
					if (i == VK_LBUTTON) {
						if (keysState[i] != keysStateOld[i]) {
							(*it)->onLeftMouseDown(Point2f((float) currentMousePos.x, (float) currentMousePos.y));
						}
					}
					// RIGHT MOUSE
					else if (i == VK_RBUTTON) {
						if (keysState[i] != keysStateOld[i]) {
							(*it)->onRightMouseDown(Point2f((float) currentMousePos.x, (float) currentMousePos.y));
						}
					}
					// KEYBOARD
					else {
						(*it)->onKeyDown(i);
					}
					it++;
				}
			}
			// Key is up
			else {
				if (keysState[i] != keysStateOld[i]) {
					std::unordered_set<InputListener*>::iterator it = listeners.begin();
					while (it != listeners.end()) {
						// LEFT MOUSE
						if (i == VK_LBUTTON) {
							(*it)->onLeftMouseUp(Point2f((float) currentMousePos.x, (float) currentMousePos.y));
						}
						// RIGHT MOUSE
						else if (i == VK_RBUTTON) {
							(*it)->onRightMouseUp(Point2f((float) currentMousePos.x, (float) currentMousePos.y));
						}
						// KEYBOARD
						else {
							(*it)->onKeyUp(i);
						}
						it++;
					}
				}
			}
		}
		// store current keys state
		memcpy(keysStateOld, keysState, sizeof(unsigned char) * 256);
	}
}

void InputSystem::addListener(InputListener* listener) {
	listeners.insert(listener);
}

void InputSystem::removeListener(InputListener* listener) {
	listeners.erase(listener);
}

void InputSystem::setCursorPosition(const Point2f& pos) {
	SetCursorPos((int) pos.x, (int) pos.y);
}

void InputSystem::showCursor(bool show) {
	ShowCursor(show);
}

void InputSystem::create() {
	if (InputSystem::inputSystem) {
		throw std::exception("InputSystem already exists.");
	}
	InputSystem::inputSystem = new InputSystem();
}

void InputSystem::release() {
	if (!InputSystem::inputSystem) {
		return;
	}
	delete InputSystem::inputSystem;
}

InputSystem* InputSystem::get() {
	return inputSystem;
}
