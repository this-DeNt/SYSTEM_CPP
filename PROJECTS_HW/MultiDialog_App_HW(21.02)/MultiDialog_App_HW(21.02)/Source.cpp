#include <windows.h>
#include <vector>
#include <string>
#include <tchar.h>
#include <fstream>
#include "resource.h"

// Function prototypes
BOOL CALLBACK MenuDialogProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK SignInDialogProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK SignUpDialogProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

// Global variables
std::vector<std::pair<std::wstring, std::wstring>> userDatabase; // Storing login and password pairs

// Entry point
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    // Show the Menu Dialog
    return DialogBox(hInstance, MAKEINTRESOURCE(IDD_MENU_DIALOG), NULL, (DLGPROC)MenuDialogProc);
}

// Menu Dialog procedure
BOOL CALLBACK MenuDialogProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
    case WM_INITDIALOG:
        return TRUE;

    case WM_COMMAND:
        switch (LOWORD(wParam)) {
        case IDC_SIGNIN_BUTTON:
            EndDialog(hwnd, 0); // Close Menu Dialog
            DialogBox(GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_SIGNIN_DIALOG), NULL, (DLGPROC)SignInDialogProc);
            return TRUE;

        case IDC_SIGNUP_BUTTON:
            EndDialog(hwnd, 0); // Close Menu Dialog
            DialogBox(GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_SIGNUP_DIALOG), NULL, (DLGPROC)SignUpDialogProc);
            return TRUE;
        }
        break;

    case WM_CLOSE:
        EndDialog(hwnd, 0);
        return TRUE;
    }
    return FALSE;
}

// SignIn Dialog procedure
BOOL CALLBACK SignInDialogProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
    case WM_INITDIALOG:
        return TRUE;

    case WM_COMMAND:
        switch (LOWORD(wParam)) {
        case IDOK: {
            // Retrieve entered login and password
            TCHAR szLogin[256], szPassword[256];
            GetDlgItemText(hwnd, IDC_LOGIN_EDIT, szLogin, 256);
            GetDlgItemText(hwnd, IDC_PASSWORD_EDIT, szPassword, 256);

            // Check if login and password match
            bool bFound = false;
            for (const auto& user : userDatabase) {
                if (user.first == szLogin && user.second == szPassword) {
                    bFound = true;
                    break;
                }
            }

            if (bFound) {
                MessageBox(hwnd, L"Login Successful", L"Success", MB_OK | MB_ICONINFORMATION);
                EndDialog(hwnd, 0); // Close SignIn Dialog
                DialogBox(GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_MENU_DIALOG), NULL, (DLGPROC)MenuDialogProc);
            }
            else {
                MessageBox(hwnd, L"Login Failed", L"Error", MB_OK | MB_ICONERROR);
                EndDialog(hwnd, 0); // Close SignIn Dialog and open SignUp Dialog
                DialogBox(GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_SIGNUP_DIALOG), NULL, (DLGPROC)SignUpDialogProc);
            }
            return TRUE;
        }
        }
        break;

    case WM_CLOSE:
        EndDialog(hwnd, 0);
        return TRUE;
    }
    return FALSE;
}

// SignUp Dialog procedure
BOOL CALLBACK SignUpDialogProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
    case WM_INITDIALOG:
        return TRUE;

    case WM_COMMAND:
        switch (LOWORD(wParam)) {
        case IDOK: {
            // Retrieve entered login, password, and email
            TCHAR szLogin[256], szPassword[256], szEmail[256];
            GetDlgItemText(hwnd, IDC_LOGIN_EDIT, szLogin, 256);
            GetDlgItemText(hwnd, IDC_PASSWORD_EDIT, szPassword, 256);
            GetDlgItemText(hwnd, IDC_EMAIL_EDIT, szEmail, 256);

            // Store the user info in the vector database
            userDatabase.push_back(std::make_pair(szLogin, szPassword));

            // Open the text file in append mode and store user info
            std::wofstream outputFile("users.txt", std::ios_base::app);
            if (outputFile.is_open()) {
                outputFile << "Login: " << szLogin << ", Password: " << szPassword << ", Email: " << szEmail << std::endl;
                outputFile.close();
            }
            else {
                MessageBox(hwnd, L"Failed to open file for writing", L"Error", MB_OK | MB_ICONERROR);
            }

            MessageBox(hwnd, L"Sign-Up Successful", L"Success", MB_OK | MB_ICONINFORMATION);
            EndDialog(hwnd, 0); // Close SignUp Dialog and open Menu Dialog
            DialogBox(GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_MENU_DIALOG), NULL, (DLGPROC)MenuDialogProc);
            return TRUE;
        }
        }
        break;

    case WM_CLOSE:
        EndDialog(hwnd, 0);
        return TRUE;
    }
    return FALSE;
}
