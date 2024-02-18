#include <windows.h>
#include <string>
#include <vector>
#include <ctime>
#include "resource.h"

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

HINSTANCE hInst;
std::vector<std::wstring> board(9, L"");
bool playerTurn = true;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
    LPSTR lpCmdLine, int nCmdShow) {

    hInst = hInstance;

    WNDCLASSEX wcex;
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(hInstance, IDI_APPLICATION);
    wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = NULL;
    wcex.lpszClassName = L"TicTacToeClass";
    wcex.hIconSm = LoadIcon(wcex.hInstance, IDI_APPLICATION);

    if (!RegisterClassEx(&wcex)) {
        MessageBox(NULL, L"Call to RegisterClassEx failed!", L"TicTacToe", NULL);
        return 1;
    }

    HWND hWnd = CreateWindow(L"TicTacToeClass", L"TicTacToe",
        WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 320, 240, NULL, NULL, hInstance, NULL);

    if (!hWnd) {
        MessageBox(NULL, L"Call to CreateWindow failed!", L"TicTacToe", NULL);
        return 1;
    }

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return (int)msg.wParam;
}

bool CheckWinner() {
    const std::vector<std::vector<int>> winningCombinations = {
        {0, 1, 2}, {3, 4, 5}, {6, 7, 8},
        {0, 3, 6}, {1, 4, 7}, {2, 5, 8},
        {0, 4, 8}, {2, 4, 6}
    };

    for (const auto& combination : winningCombinations) {
        if (board[combination[0]] != L"" &&
            board[combination[0]] == board[combination[1]] &&
            board[combination[1]] == board[combination[2]]) {
            return true;
        }
    }

    return false;
}

void ComputerMove() {
    std::vector<int> availableMoves;
    for (int i = 0; i < 9; ++i) {
        if (board[i] == L"") {
            availableMoves.push_back(i);
        }
    }

    int randomIndex = rand() % availableMoves.size();
    int computerMoveIndex = availableMoves[randomIndex];
    board[computerMoveIndex] = L"O";

    HWND hwndButton = GetDlgItem(GetActiveWindow(), IDC_BUTTON1 + computerMoveIndex);
    SetWindowText(hwndButton, L"O");

    if (CheckWinner()) {
        MessageBox(GetActiveWindow(), L"Computer wins!", L"Game Over", MB_OK);
        PostQuitMessage(0);
    }

    bool isBoardFull = true;
    for (const auto& cell : board) {
        if (cell == L"") {
            isBoardFull = false;
            break;
        }
    }
    if (isBoardFull) {
        MessageBox(GetActiveWindow(), L"It's a tie!", L"Game Over", MB_OK);
        PostQuitMessage(0);
    }

    playerTurn = true;
}

void OnButtonClick(int buttonId) {
    if (playerTurn) {
        int boardIndex = buttonId - IDC_BUTTON1;
        if (board[boardIndex] == L"") {
            board[boardIndex] = L"X";
            HWND hwndButton = GetDlgItem(GetActiveWindow(), buttonId);
            SetWindowText(hwndButton, L"X");

            if (CheckWinner()) {
                MessageBox(GetActiveWindow(), L"Player wins!", L"Game Over", MB_OK);
                PostQuitMessage(0);
            }

            playerTurn = false;
            ComputerMove();
        }
    }
}

void StartNewGame() {
    for (auto& cell : board) {
        cell = L"";
    }

    for (int i = IDC_BUTTON1; i <= IDC_BUTTON9; ++i) {
        HWND hwndButton = GetDlgItem(GetActiveWindow(), i);
        SetWindowText(hwndButton, L"");
    }

    playerTurn = true;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
    case WM_CREATE:
        CreateWindow(L"BUTTON", L"", WS_VISIBLE | WS_CHILD, 12, 12, 41, 30, hWnd, (HMENU)IDC_BUTTON1, hInst, NULL);
        CreateWindow(L"BUTTON", L"", WS_VISIBLE | WS_CHILD, 108, 12, 41, 30, hWnd, (HMENU)IDC_BUTTON2, hInst, NULL);
        CreateWindow(L"BUTTON", L"", WS_VISIBLE | WS_CHILD, 60, 12, 41, 30, hWnd, (HMENU)IDC_BUTTON3, hInst, NULL);
        CreateWindow(L"BUTTON", L"", WS_VISIBLE | WS_CHILD, 12, 48, 41, 30, hWnd, (HMENU)IDC_BUTTON4, hInst, NULL);
        CreateWindow(L"BUTTON", L"", WS_VISIBLE | WS_CHILD, 108, 48, 41, 30, hWnd, (HMENU)IDC_BUTTON5, hInst, NULL);
        CreateWindow(L"BUTTON", L"", WS_VISIBLE | WS_CHILD, 60, 48, 41, 30, hWnd, (HMENU)IDC_BUTTON6, hInst, NULL);
        CreateWindow(L"BUTTON", L"", WS_VISIBLE | WS_CHILD, 12, 84, 41, 30, hWnd, (HMENU)IDC_BUTTON7, hInst, NULL);
        CreateWindow(L"BUTTON", L"", WS_VISIBLE | WS_CHILD, 108, 84, 41, 30, hWnd, (HMENU)IDC_BUTTON8, hInst, NULL);
        CreateWindow(L"BUTTON", L"", WS_VISIBLE | WS_CHILD, 60, 84, 41, 30, hWnd, (HMENU)IDC_BUTTON9, hInst, NULL);
        CreateWindow(L"BUTTON", L"New Game", WS_VISIBLE | WS_CHILD, 12, 130, 80, 30, hWnd, (HMENU)IDC_NEW_GAME_BUTTON, hInst, NULL);
        break;
    case WM_COMMAND:
    {
        int wmId = LOWORD(wParam);
        switch (wmId)
        {
        case IDC_BUTTON1:
        case IDC_BUTTON2:
        case IDC_BUTTON3:
        case IDC_BUTTON4:
        case IDC_BUTTON5:
        case IDC_BUTTON6:
        case IDC_BUTTON7:
        case IDC_BUTTON8:
        case IDC_BUTTON9:
            OnButtonClick(wmId);
            break;
        case IDC_NEW_GAME_BUTTON:
            StartNewGame();
            break;
        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
        }
    }
    break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}
