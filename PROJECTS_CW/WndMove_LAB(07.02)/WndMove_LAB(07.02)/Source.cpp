#include <windows.h>

HWND hWnd;
bool moving = false;
int speed = 10;

LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    WNDCLASS wc = {};
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = L"MovingWindowApp";

    RegisterClass(&wc);

    hWnd = CreateWindowEx(
        0,
        L"MovingWindowApp",
        L"Moving Window App",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 200, 200,
        NULL,
        NULL,
        hInstance,
        NULL
    );

    ShowWindow(hWnd, nCmdShow);

    MSG msg = {};
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}

LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
    case WM_KEYDOWN:
        switch (wParam) {
        case VK_RETURN:
            if (!moving) {
                moving = true;
                SetTimer(hWnd, 1, speed, NULL);
            }
            break;
        case VK_ESCAPE:
            moving = false;
            KillTimer(hWnd, 1);
            break;
        }
        break;
    case WM_TIMER:
        if (moving) {
            RECT rect;
            GetWindowRect(hWnd, &rect);
            int x = rect.left;
            int y = rect.top;

            int screenWidth = GetSystemMetrics(SM_CXSCREEN);
            int screenHeight = GetSystemMetrics(SM_CYSCREEN);

            if (x < screenWidth - 300 && y == 0)
                SetWindowPos(hWnd, HWND_TOP, x + 10, y, 0, 0, SWP_NOSIZE);
            else if (x >= screenWidth - 300 && y < screenHeight - 300)
                SetWindowPos(hWnd, HWND_TOP, x, y + 10, 0, 0, SWP_NOSIZE);
            else if (x > 0 && y >= screenHeight - 300)
                SetWindowPos(hWnd, HWND_TOP, x - 10, y, 0, 0, SWP_NOSIZE);
            else
                SetWindowPos(hWnd, HWND_TOP, x, y - 10, 0, 0, SWP_NOSIZE);
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, uMsg, wParam, lParam);
    }
    return 0;
}
