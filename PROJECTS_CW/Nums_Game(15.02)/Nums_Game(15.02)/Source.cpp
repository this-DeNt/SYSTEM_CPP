#include <windows.h>
#include <ctime>
#include <vector>
#include <string>
#include <tchar.h>
#include "resource.h"

// Global Variables
HWND hButton[16];
HWND hListBox, hNewGameButton, hProgress, hTimeEdit, hTimeSpin;
std::vector<int> numbers(16);
int currentTime, targetTime;

// Function Declarations
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
BOOL CALLBACK DialogProc(HWND, UINT, WPARAM, LPARAM);
void GenerateRandomNumbers();
void UpdateListBox();
void StartNewGame();
void CheckNumber(int index);

// Entry Point
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    // Create dialog box
    DialogBox(hInstance, MAKEINTRESOURCE(IDD_DIALOG1), NULL, (DLGPROC)DialogProc);

    return 0;
}

// Dialog Procedure
BOOL CALLBACK DialogProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
    case WM_INITDIALOG:
        // Initialize controls
        for (int i = 0; i < 16; ++i) {
            hButton[i] = GetDlgItem(hDlg, IDC_BUTTON2 + i);
        }
        hListBox = GetDlgItem(hDlg, IDC_LIST1);
        hNewGameButton = GetDlgItem(hDlg, IDC_BUTTON18);
        hProgress = GetDlgItem(hDlg, IDC_PROGRESS1);
        hTimeEdit = GetDlgItem(hDlg, IDC_EDIT2);
        hTimeSpin = GetDlgItem(hDlg, IDC_SPIN3);

        SendMessage(hProgress, PBM_SETRANGE, 0, MAKELPARAM(0, 100));
        SendMessage(hTimeSpin, UDM_SETRANGE, 0, MAKELPARAM(100, 1));

        // Set initial values
        SendMessage(hTimeEdit, WM_SETTEXT, 0, (LPARAM)_T("30"));

        // Generate random numbers
        GenerateRandomNumbers();
        UpdateListBox();
        return TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDC_BUTTON18) {
            StartNewGame();
        }
        else {
            for (int i = 0; i < 16; ++i) {
                if (hButton[i] == (HWND)lParam) {
                    CheckNumber(i);
                    break;
                }
            }
        }
        return TRUE;

    case WM_CLOSE:
        EndDialog(hDlg, 0);
        return TRUE;
    }
    return FALSE;
}

// Main Window Procedure
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
    case WM_COMMAND:
        break;
    case WM_PAINT:
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// Generate Random Numbers
void GenerateRandomNumbers() {
    srand(time(NULL));
    for (int i = 0; i < 16; ++i) {
        numbers[i] = rand() % 101;
    }
}

// Update List Box
void UpdateListBox() {
    SendMessage(hListBox, LB_RESETCONTENT, 0, 0);
    for (int i = 0; i < 16; ++i) {
        TCHAR buffer[10];
        _itot_s(numbers[i], buffer, 10);
        SendMessage(hListBox, LB_ADDSTRING, 0, (LPARAM)buffer);
    }
}

// Start New Game
void StartNewGame() {
    GenerateRandomNumbers();
    UpdateListBox();
    SendMessage(hProgress, PBM_SETPOS, 0, 0);

    TCHAR buffer[10];
    GetWindowText(hTimeEdit, buffer, 10);
    targetTime = _ttoi(buffer);
    currentTime = 0;
    SendMessage(hProgress, PBM_SETPOS, 0, 0);
}

// Check Number
void CheckNumber(int index) {
    int currentNumber = numbers[index];
    if (index == 0 || currentNumber > numbers[index - 1]) {
        SendMessage(hListBox, LB_SETCURSEL, index, 0);
        SendMessage(hProgress, PBM_STEPIT, 0, 0);
        if (currentTime++ >= targetTime) {
            MessageBox(NULL, _T("Time's up!"), _T("Game Over"), MB_OK | MB_ICONINFORMATION);
            StartNewGame();
        }
    }
}
