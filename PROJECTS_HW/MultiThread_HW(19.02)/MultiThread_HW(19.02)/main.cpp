#include <windows.h>
#include <process.h>
#include <vector>
#include <ctime>
#include <string>

#define IDC_BUTTON1 101
#define IDC_BUTTON2 102
#define IDC_BUTTON3 103
#define IDC_BUTTON4 104
#define IDC_BUTTON5 105
#define IDC_BUTTON6 106
#define IDC_LIST1   107
#define IDC_LIST2   108
#define IDC_LIST3   109

HWND hListBox1, hListBox2, hListBox3;
HANDLE hThread1, hThread2, hThread3;
volatile bool bGenerateEvens = false;
volatile bool bGenerateOdds = false;
volatile bool bGenerateRandoms = false;

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

unsigned __stdcall GenerateEvens(void* /*data*/) {
    while (bGenerateEvens) {
        int num = rand() % 100;
        if (num % 2 == 0) {
            wchar_t buffer[16];
            _itow_s(num, buffer, 10);
            SendMessage(hListBox1, LB_ADDSTRING, 0, (LPARAM)buffer);
        }
        Sleep(1000);
    }
    return 0;
}

unsigned __stdcall GenerateOdds(void* /*data*/) {
    while (bGenerateOdds) {
        int num = rand() % 100;
        if (num % 2 != 0) {
            wchar_t buffer[16];
            _itow_s(num, buffer, 10);
            SendMessage(hListBox2, LB_ADDSTRING, 0, (LPARAM)buffer);
        }
        Sleep(1000);
    }
    return 0;
}

unsigned __stdcall GenerateRandoms(void* /*data*/) {
    while (bGenerateRandoms) {
        int num = rand() % 100;
        wchar_t buffer[16];
        _itow_s(num, buffer, 10);
        SendMessage(hListBox3, LB_ADDSTRING, 0, (LPARAM)buffer);
        Sleep(1000);
    }
    return 0;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow) {
    static TCHAR szAppName[] = TEXT("MultiThreadedApp");
    HWND hwnd;
    MSG msg;
    WNDCLASS wndclass;

    wndclass.style = CS_HREDRAW | CS_VREDRAW;
    wndclass.lpfnWndProc = WndProc;
    wndclass.cbClsExtra = 0;
    wndclass.cbWndExtra = 0;
    wndclass.hInstance = hInstance;
    wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
    wndclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
    wndclass.lpszMenuName = NULL;
    wndclass.lpszClassName = szAppName;

    if (!RegisterClass(&wndclass)) {
        MessageBox(NULL, TEXT("This program requires Windows NT!"), szAppName, MB_ICONERROR);
        return 0;
    }

    hwnd = CreateWindow(szAppName, TEXT("Multi-Threaded App"), WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 600, 400, NULL, NULL, hInstance, NULL);

    ShowWindow(hwnd, iCmdShow);
    UpdateWindow(hwnd);

    srand(static_cast<unsigned int>(time(nullptr)));

    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
    case WM_CREATE:
        hListBox1 = CreateWindow(TEXT("LISTBOX"), NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | LBS_SORT | LBS_NOINTEGRALHEIGHT | WS_VSCROLL,
            126, 12, 48, 40, hwnd, (HMENU)IDC_LIST1, ((LPCREATESTRUCT)lParam)->hInstance, NULL);
        hListBox2 = CreateWindow(TEXT("LISTBOX"), NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | LBS_SORT | LBS_NOINTEGRALHEIGHT | WS_VSCROLL,
            126, 66, 48, 40, hwnd, (HMENU)IDC_LIST2, ((LPCREATESTRUCT)lParam)->hInstance, NULL);
        hListBox3 = CreateWindow(TEXT("LISTBOX"), NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | LBS_SORT | LBS_NOINTEGRALHEIGHT | WS_VSCROLL,
            126, 120, 48, 40, hwnd, (HMENU)IDC_LIST3, ((LPCREATESTRUCT)lParam)->hInstance, NULL);
        CreateWindow(TEXT("BUTTON"), TEXT("Start/Stop(Evens)"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
            12, 12, 100, 25, hwnd, (HMENU)IDC_BUTTON1, ((LPCREATESTRUCT)lParam)->hInstance, NULL);
        CreateWindow(TEXT("BUTTON"), TEXT("Terminate(Evens)"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
            12, 42, 100, 25, hwnd, (HMENU)IDC_BUTTON2, ((LPCREATESTRUCT)lParam)->hInstance, NULL);
        CreateWindow(TEXT("BUTTON"), TEXT("Start/Stop(Odds)"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
            12, 66, 100, 25, hwnd, (HMENU)IDC_BUTTON3, ((LPCREATESTRUCT)lParam)->hInstance, NULL);
        CreateWindow(TEXT("BUTTON"), TEXT("Terminate(Odds)"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
            12, 96, 100, 25, hwnd, (HMENU)IDC_BUTTON4, ((LPCREATESTRUCT)lParam)->hInstance, NULL);
        CreateWindow(TEXT("BUTTON"), TEXT("Start/Stop(Rands)"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
            12, 120, 100, 25, hwnd, (HMENU)IDC_BUTTON5, ((LPCREATESTRUCT)lParam)->hInstance, NULL);
        CreateWindow(TEXT("BUTTON"), TEXT("Terminate(Rands)"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
            12, 150, 100, 25, hwnd, (HMENU)IDC_BUTTON6, ((LPCREATESTRUCT)lParam)->hInstance, NULL);
        break;
    case WM_COMMAND:
        switch (LOWORD(wParam)) {
        case IDC_BUTTON1: // Start/Stop(Evens)
            bGenerateEvens = !bGenerateEvens;
            if (bGenerateEvens) {
                hThread1 = (HANDLE)_beginthreadex(nullptr, 0, GenerateEvens, nullptr, 0, nullptr);
            }
            else {
                TerminateThread(hThread1, 0);
            }
            break;
        case IDC_BUTTON2: // Terminate(Evens)
            TerminateThread(hThread1, 0);
            EnableWindow(GetDlgItem(hwnd, IDC_BUTTON1), FALSE);
            break;
        case IDC_BUTTON3: // Start/Stop(Odds)
            bGenerateOdds = !bGenerateOdds;
            if (bGenerateOdds) {
                hThread2 = (HANDLE)_beginthreadex(nullptr, 0, GenerateOdds, nullptr, 0, nullptr);
            }
            else {
                TerminateThread(hThread2, 0);
            }
            break;
        case IDC_BUTTON4: // Terminate(Odds)
            TerminateThread(hThread2, 0);
            EnableWindow(GetDlgItem(hwnd, IDC_BUTTON3), FALSE);
            break;
        case IDC_BUTTON5: // Start/Stop(Rands)
            bGenerateRandoms = !bGenerateRandoms;
            if (bGenerateRandoms) {
                hThread3 = (HANDLE)_beginthreadex(nullptr, 0, GenerateRandoms, nullptr, 0, nullptr);
            }
            else {
                TerminateThread(hThread3, 0);
            }
            break;
        case IDC_BUTTON6: // Terminate(Rands)
            TerminateThread(hThread3, 0);
            EnableWindow(GetDlgItem(hwnd, IDC_BUTTON5), FALSE);
            break;
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hwnd, message, wParam, lParam);
    }
    return 0;
}
