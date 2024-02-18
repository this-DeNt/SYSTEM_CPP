#include <windows.h>
#include <vector>
#include <string>

const int MIN_SIZE = 10;

// Define a structure to represent a static control
struct StaticControl {
    HWND hwnd;
    int x, y, width, height;
    int id;
};

// Declare global variables
std::vector<StaticControl> staticControls;
HWND hwnd;

// Define a message handler for the dialog
BOOL CALLBACK DialogProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
    case WM_LBUTTONDOWN: {
        // User clicked the left mouse button
        POINT pt;
        GetCursorPos(&pt);
        ScreenToClient(hwnd, &pt);

        // Create a new static control
        HWND newStatic = CreateWindowEx(0, "STATIC", NULL, WS_VISIBLE | WS_CHILD,
            pt.x, pt.y, MIN_SIZE, MIN_SIZE, hwnd, NULL, GetModuleHandle(NULL), NULL);

        if (newStatic != NULL) {
            staticControl.push_back({ newStatic, pt.x, pt.y, MIN_SIZE, MIN_SIZE, staticControls.size() });
            ShowWindow(staticControls.back().hwnd, SW_SHOW);
        }
        else {
            MessageBox(hwnd, "Error creating static control", "Error", MB_OK);
        }

        break;
    }

    case WM_MOUSEMOVE: {
        // User is moving the mouse
        POINT pt;
        GetCursorPos(&pt);
        ScreenToClient(hwnd, &pt);

        if (wParam & MK_LBUTTON) {
            // Left button is down, move the last static control
            if (!staticControls.empty()) {
                staticControl& lastStatic = staticControls.back();
                lastStatic.width = pt.x - lastStatic.x;
                lastStatic.height = pt.y - lastStatic.y;

                MoveWindow(lastStatic.hwnd, lastStatic.x, lastStatic.y, lastStatic.width, lastStatic.height, TRUE);
            }
        }

        break;
    }

    case WM_RBUTTONUP: {
        // User clicked the right mouse button
        POINT pt;
        GetCursorPos(&pt);
        ScreenToClient(hwnd, &pt);

        for (StaticControl& staticControl : staticControls) {
            if (PtInRect(&staticControl.rect, pt)) {
                // Right-clicked on a static control
                std::stringstream ss;
                ss << "Static " << staticControl.id << ":\n"
                    << "Width: " << staticControl.width << "\n"
                    << "Height: " << staticControl.height << "\n"
                    << "X: " << staticControl.x << "\n"
                    << "Y: " << staticControl.y << "\n";

                MessageBox(hwnd, ss.str().c_str(), "Static Info", MB_OK);
                break;
            }
        }

        break;
    }

    case WM_LBUTTONDBLCLK: {
        // User double-clicked the left mouse button
        POINT pt;
        GetCursorPos(&pt);
        ScreenToClient(hwnd, &pt);

        for (StaticControl& staticControl : staticControls) {
            if (PtInRect(&staticControl.rect, pt)) {
                // Double-clicked on a static control
                DestroyWindow(staticControl.hwnd);
                staticControls.erase(std::remove(staticControls.begin(), staticControls.end(), staticControl),
                    staticControls.end());
                break;
            }
        }

        break;
    }

    case WM_DESTROY: {
        // Clean up static controls
        for (StaticControl& staticControl : staticControls) {
            DestroyWindow(staticControl.hwnd);
        }
