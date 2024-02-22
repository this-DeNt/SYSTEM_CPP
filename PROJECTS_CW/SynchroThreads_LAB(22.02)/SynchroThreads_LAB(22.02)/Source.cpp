#include <windows.h>
#include <ctime>
#include <cstdlib>
#include <string>
#include <tchar.h>
#include "resource.h"

HWND hDlg, hListBox;
int idx = 0;
int numbers[10];

DWORD WINAPI FirstThread(LPVOID lpParam) {
    for (int i = 0; i < 10; ++i) {
        int newValue = rand() % 100;
        InterlockedExchange(reinterpret_cast<LONG*>(&numbers[idx++]), static_cast<LONG>(newValue));
        Sleep(100);
    }
    MessageBox(hDlg, _T("Writing to array done!"), _T("Thread 1"), MB_OK);
    return 0;
}

DWORD WINAPI SecondThread(LPVOID lpParam) {
    while (idx < 10) {
        // Wait until the first thread writes at least one value
        Sleep(100);
    }
    SendMessage(hListBox, LB_RESETCONTENT, 0, 0);
    for (int i = 0; i < 10; ++i) {
        TCHAR buffer[10];
        _stprintf_s(buffer, _T("%d"), numbers[i]);
        SendMessage(hListBox, LB_ADDSTRING, 0, (LPARAM)buffer);
        Sleep(100);
    }
    MessageBox(hDlg, _T("Reading from array and writing to list box done!"), _T("Thread 2"), MB_OK);
    return 0;
}

INT_PTR CALLBACK DialogProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
    case WM_INITDIALOG:
        hDlg = hwndDlg;
        hListBox = GetDlgItem(hwndDlg, IDC_LIST1);
        {
            DWORD firstThreadId;
            HANDLE hFirstThread = CreateThread(NULL, 0, FirstThread, NULL, 0, &firstThreadId);
            CloseHandle(hFirstThread);
        }
        {
            DWORD secondThreadId;
            HANDLE hSecondThread = CreateThread(NULL, 0, SecondThread, NULL, 0, &secondThreadId);
            CloseHandle(hSecondThread);
        }
        return TRUE;

    case WM_CLOSE:
        EndDialog(hwndDlg, 0);
        break;
    }
    return FALSE;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    srand(static_cast<unsigned int>(time(NULL)));
    DialogBox(hInstance, MAKEINTRESOURCE(IDD_DIALOG1), NULL, DialogProc);
    return 0;
}
