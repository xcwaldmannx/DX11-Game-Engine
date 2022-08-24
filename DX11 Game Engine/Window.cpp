#include "Window.h"

#include <exception>

// Window* window = nullptr;

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
    switch (msg) {
    case WM_CREATE: {
        break;
    }
    case WM_SIZE: {
        Window* window = (Window*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
        if (window) {
            window->onSize();
        }
        break;
    }
    case WM_GETMINMAXINFO:  {
        // Event fired when the window is resized
        // and stops at the width / height specified below
        LPMINMAXINFO minmax = (LPMINMAXINFO)lparam;
        minmax->ptMinTrackSize.x = 640;
        minmax->ptMinTrackSize.y = 480;
        break;
    }
    case WM_DESTROY: {
        Window* window = (Window*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
        window->onDestroy();
        PostQuitMessage(0);
        break;
    }
    case WM_SETFOCUS: {
        Window* window = (Window*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
        if (window) {
            window->onFocus();
        }
        break;
    }
    case WM_KILLFOCUS: {
        Window* window = (Window*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
        window->onKillFocus();
        break;
    }
    default:
        return DefWindowProc(hwnd, msg, wparam, lparam);
    }

    return NULL;
}

Window::Window() {
    WNDCLASSEX wc;
    wc.cbClsExtra = NULL;
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.cbWndExtra = NULL;
    wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
    wc.hInstance = NULL;
    wc.lpfnWndProc = &WndProc;
    wc.lpszClassName = L"MyWindowClass";
    wc.lpszMenuName = L"";
    wc.style = NULL;

    if (!RegisterClassEx(&wc)) {
        throw std::exception("Window could not be created.");
    }

    /*if (!window) {
        window = this;
    }*/

    hwnd = CreateWindowEx(WS_EX_OVERLAPPEDWINDOW, L"MyWindowClass", L"Title",
        WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 1024, 768, NULL, NULL, NULL, NULL);

    if (!hwnd) {
        throw std::exception("Window could not be created.");
    }

    ShowWindow(hwnd, SW_SHOW);
    UpdateWindow(hwnd);

    isWindowRunning = true;

}

Window::~Window() {
}

bool Window::broadcast() {

    if (!isInit) {
        SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR) this);
        this->onCreate();
        isInit = true;
    }


    this->onUpdate();

    MSG msg;
    while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) > 0) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    Sleep(1);

    return true;
}

bool Window::isRunning() {
    if (isWindowRunning) {
        broadcast();
    }
    return isWindowRunning;
}

RECT Window::getWindowRect() {
    RECT r;
    GetClientRect(hwnd, &r);
    return r;
}

RECT Window::getWindowPosition() {
    RECT r;
    GetWindowRect(hwnd, &r);
    return r;
}

RECT Window::getScreenSize() {
    RECT rc;
    rc.left = 0;
    rc.bottom = 0;
    rc.right = GetSystemMetrics(SM_CXSCREEN);
    rc.bottom = GetSystemMetrics(SM_CYSCREEN);
    return rc;
}

void Window::onCreate() {
}

void Window::onUpdate() {
}

void Window::onDestroy() {
    isWindowRunning = false;
}

void Window::onFocus() {

}

void Window::onKillFocus() {

}

void Window::onSize() {

}
