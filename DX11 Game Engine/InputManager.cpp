#include "InputManager.h"

InputManager::InputManager() {
	for (int i = 0; i < 256; i++) {
		keyHoldStates[i] = 0.0f;
		keyPressStates[i] = 0.0f;
		keyTapStates[i] = -1.0f;
	}

	lmbState = 0.0f;
	mmbState = 0.0f;
	rmbState = 0.0f;
}

InputManager::~InputManager() {
}

bool InputManager::getKeyHoldState(char key, float duration) {
	return keyHoldStates[key] >= duration;
}

float InputManager::getKeyPressState(char key) {
	return keyPressStates[key];
}

float InputManager::getKeyTapState(char key) {
	float value = keyTapStates[key];
	keyTapStates[key] = -1.0f;
	return value;
}

float InputManager::getMouseState(MOUSE_STATE state) {
	switch (state) {
	case LMB_STATE:
		return lmbState;
	case MMB_STATE:
		return mmbState;
	case RMB_STATE:
		return rmbState;
	default:
		return 0.0f;
	}
}

const Point2f& InputManager::getMousePosition() {
	return position;
}

void InputManager::onKeyDown(int key) {
	keyHoldStates[key]++;
	keyPressStates[key] = 1.0f;
	keyTapStates[key] = 1.0f;
}
void InputManager::onKeyUp(int key) {
	keyHoldStates[key] = 0.0f;
	keyPressStates[key] = 0.0f;
	keyTapStates[key] = 0.0f;
}

#include <iostream>

void InputManager::onMouseMove(const Point2f& mousePos) {
	position.x = mousePos.x;
	position.y = mousePos.y;
}

void InputManager::onLeftMouseDown(const Point2f& mousePos) {
	lmbState = 1.0f;
}
void InputManager::onLeftMouseUp(const Point2f& mousePos) {
	lmbState = 0.0f;
}

void InputManager::onRightMouseDown(const Point2f& mousePos) {
	rmbState = 1.0f;
}
void InputManager::onRightMouseUp(const Point2f& mousePos) {
	rmbState = 0.0f;
}
