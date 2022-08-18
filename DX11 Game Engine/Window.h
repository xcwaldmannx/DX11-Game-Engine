#pragma once

#include <Windows.h>

class Window {
public:
    Window();
    ~Window();

    // window is running
    bool isRunning();

    // return window dimensions
    RECT getWindowRect();
    RECT getWindowPosition();
    RECT getScreenSize();

    // events
    virtual void onCreate();
    virtual void onUpdate();
    virtual void onDestroy();
    virtual void onFocus();
    virtual void onKillFocus();
    virtual void onSize();

private:
    // window event handler
    bool broadcast();

protected:
    HWND hwnd{};
    bool isWindowRunning = false;
    bool isInit = false;
};
