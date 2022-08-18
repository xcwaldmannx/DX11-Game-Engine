#include "AppWindow.h"
#include "InputSystem.h"

int main() {
    std::cout << "Running..." << std::endl;

    try {
        GraphicsEngine::create();
        InputSystem::create();
    }
    catch (...) {
        return -1;
    }

    try {
        AppWindow app;
        while (app.isRunning());
    } catch (...) {
        InputSystem::release();
        GraphicsEngine::release();
        return -1;
    }

    InputSystem::release();
    GraphicsEngine::release();

    return 0;
};
