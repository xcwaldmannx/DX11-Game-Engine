#include "InputManager.h"

InputManager::InputManager() {
	for (int i = 0; i < 128; i++) {
		keyHoldStates.insert({i, 0.0f});
		keyTapStates.insert({i, -1.0f});
	}

	lmbState = 0.0f;
	mmbState = 0.0f;
	rmbState = 0.0f;
}

InputManager::~InputManager() {
}

float InputManager::getKeyHoldState(char key) {
	return keyHoldStates[key];
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
	std::unordered_map<char, float>::iterator hold = keyHoldStates.begin();
	while (hold != keyHoldStates.end()) {
		if (hold->first == key) {
			hold->second = 1.0f;
		}
		hold++;
	}

	std::unordered_map<char, float>::iterator tap = keyTapStates.begin();
	while (tap != keyTapStates.end()) {
		if (tap->first == key) {
			tap->second = 1.0f;
		}
		tap++;
	}
}
void InputManager::onKeyUp(int key) {
	std::unordered_map<char, float>::iterator hold = keyHoldStates.begin();
	while (hold != keyHoldStates.end()) {
		if (hold->first == key) {
			hold->second = 0.0f;
		}
		hold++;
	}

	std::unordered_map<char, float>::iterator tap = keyTapStates.begin();
	while (tap != keyTapStates.end()) {
		if (tap->first == key) {
			tap->second = 0.0f;
		}
		tap++;
	}
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
