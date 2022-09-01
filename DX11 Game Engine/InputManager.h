#pragma once

#include <unordered_map>

#include "InputListener.h"
#include "InputSystem.h"

#include "Point2f.h"

typedef int MOUSE_STATE;

class InputManager : public InputListener {
public:
    InputManager();
    ~InputManager();

    bool getKeyHoldState(char key, float duration);
    float getKeyPressState(char key);
    float getKeyTapState(char key);

    float getMouseState(MOUSE_STATE state);
    const Point2f& getMousePosition();

    virtual void onKeyDown(int key) override;
    virtual void onKeyUp(int key) override;

    virtual void onMouseMove(const Point2f& mousePos) override;
    virtual void onLeftMouseDown(const Point2f& mousePos) override;
    virtual void onLeftMouseUp(const Point2f& mousePos) override;
    virtual void onRightMouseDown(const Point2f& mousePos) override;
    virtual void onRightMouseUp(const Point2f& mousePos) override;

public:
    const static MOUSE_STATE LMB_STATE  = 1;
    const static MOUSE_STATE MMB_STATE  = 2;
    const static MOUSE_STATE RMB_STATE  = 3;

private:
    // Tracks the duration a key was held for.
    float keyHoldStates[256] = {};
    // Tracks if a key is up or down.
    float keyPressStates[256] = {};
    // Tracks if a key is tapped and released.
    float keyTapStates[256] = {};

    Point2f position;
    Point2f delta;

    float lmbState;
    float mmbState;
    float rmbState;
};