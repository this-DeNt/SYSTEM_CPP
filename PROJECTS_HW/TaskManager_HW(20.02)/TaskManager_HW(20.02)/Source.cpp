#include <windows.h>
#include <psapi.h>
#include <vector>
#include <string>
#include <tchar.h>
#include "resource.h"
#include <tlhelp32.h>
#include <iostream>

// Link with the Psapi.lib file
#pragma comment(lib, "Psapi.lib")

// Global variables
HWND hListBox, hEditBox;
std::vector<std::wstring> runningProcesses;

// Function to update the list of running processes in the ListBox
void UpdateProcessList() {
    // Clear the existing process list
    SendMessage(hListBox, LB_RESETCONTENT, 0, 0);

    // Get the list of process identifiers
    DWORD processIds[1024], bytesReturned;
    if (EnumProcesses(processIds, sizeof(processIds), &bytesReturned)) {
        // Calculate the number of processes
        DWORD numProcesses = bytesReturned / sizeof(DWORD);
        for (DWORD i = 0; i < numProcesses; ++i) {
            // Get a handle to the process
            HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, processIds[i]);
            if (hProcess != NULL) {
                // Get the process name
                TCHAR processName[MAX_PATH];
                if (GetModuleBaseName(hProcess, NULL, processName, MAX_PATH) > 0) {
                    // Add the process name to the list
                    std::wstring processNameStr(processName);
                    SendMessage(hListBox, LB_ADDSTRING, 0, (LPARAM)processNameStr.c_str());
                }
                // Close the process handle
                CloseHandle(hProcess);
            }
        }
    }
}

// Function to handle the "Terminate Process" button click
void TerminateSelectedProcess() {
    // Get the index of the selected item in the ListBox
    int selectedIndex = SendMessage(hListBox, LB_GETCURSEL, 0, 0);
    if (selectedIndex != LB_ERR) {
        // Get the process name from the selected item
        TCHAR processName[MAX_PATH];
        SendMessage(hListBox, LB_GETTEXT, selectedIndex, (LPARAM)processName);

        // Iterate through the running processes to find the process ID of the selected process
        DWORD processId = 0;
        HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
        if (hSnapshot != INVALID_HANDLE_VALUE) {
            PROCESSENTRY32 pe32;
            pe32.dwSize = sizeof(PROCESSENTRY32);
            if (Process32First(hSnapshot, &pe32)) {
                do {
                    if (_tcscmp(pe32.szExeFile, processName) == 0) {
                        processId = pe32.th32ProcessID;
                        break;
                    }
                } while (Process32Next(hSnapshot, &pe32));
            }
            CloseHandle(hSnapshot);
        }

        // Terminate the selected process
        if (processId != 0) {
            HANDLE hProcess = OpenProcess(PROCESS_TERMINATE, FALSE, processId);
            if (hProcess != NULL) {
                TerminateProcess(hProcess, 0);
                CloseHandle(hProcess);
                // Update the process list to reflect the changes
                UpdateProcessList();
            }
        }
    }
}

// Function to create a new process based on the path entered in the edit control
void CreateNewProcess() {
    // Get the text from the edit control
    TCHAR buffer[MAX_PATH];
    GetWindowText(hEditBox, buffer, MAX_PATH);

    // Create process
    STARTUPINFO si = { sizeof(si) };
    PROCESS_INFORMATION pi;
    BOOL success = CreateProcess(NULL, buffer, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi);

    if (success) {
        // Wait for the process to finish
        WaitForSingleObject(pi.hProcess, INFINITE);
        // Close process and thread handles
        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);
        // Update the process list to reflect the changes
        UpdateProcessList();
    }
    else {
        MessageBox(NULL, TEXT("Failed to create process."), TEXT("Error"), MB_OK | MB_ICONERROR);
    }
}

//std::wstring GetProcessInfo(DWORD processId) {
//    HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, processId);
//    if (hProcess == NULL) {
//        return L"Error: Unable to open process";
//    }
//
//    TCHAR processName[MAX_PATH];
//    if (GetModuleFileNameEx(hProcess, NULL, processName, MAX_PATH) == 0) {
//        CloseHandle(hProcess);
//        return L"Error: Unable to get process name";
//    }
//
//    // Get process memory usage
//    PROCESS_MEMORY_COUNTERS_EX pmc;
//    if (!GetProcessMemoryInfo(hProcess, (PROCESS_MEMORY_COUNTERS*)&pmc, sizeof(pmc))) {
//        CloseHandle(hProcess);
//        return L"Error: Unable to get process memory info";
//    }
//
//    // Get number of threads
//    DWORD numThreads = 0;
//    HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0);
//    if (hSnapshot != INVALID_HANDLE_VALUE) {
//        THREADENTRY32 te32;
//        te32.dwSize = sizeof(THREADENTRY32);
//        if (Thread32First(hSnapshot, &te32)) {
//            do {
//                if (te32.th32OwnerProcessID == processId) {
//                    ++numThreads;
//                }
//            } while (Thread32Next(hSnapshot, &te32));
//        }
//        CloseHandle(hSnapshot);
//    }
//
//    CloseHandle(hProcess);
//
//    // Construct process info string
//    std::wstring info = processName;
//    info += L"\nProcess ID: " + std::to_wstring(processId);
//    info += L"\nMemory Usage: " + std::to_wstring(pmc.WorkingSetSize / (1024 * 1024)) + L" MB";
//    info += L"\nNumber of Threads: " + std::to_wstring(numThreads);
//
//    return info;
//}
//
//// Function to show process information in a console window
//void ShowProcessInfo() {
//    // Create a console window
//    AllocConsole();
//
//    // Redirect standard output to console
//    FILE* pConsoleStream;
//    if (freopen_s(&pConsoleStream, "CONOUT$", "w", stdout) != 0) {
//        FreeConsole();
//        return;
//    }
//
//    // Get the list of process identifiers
//    DWORD processIds[1024], bytesReturned;
//    if (EnumProcesses(processIds, sizeof(processIds), &bytesReturned)) {
//        // Calculate the number of processes
//        DWORD numProcesses = bytesReturned / sizeof(DWORD);
//        for (DWORD i = 0; i < numProcesses; ++i) {
//            // Get detailed information about each process
//            std::wstring processInfo = GetProcessInfo(processIds[i]);
//            // Display process info in the console window
//            std::wcout << processInfo << std::endl;
//        }
//    }
//
//    // Release the console window
//    FreeConsole();
//}

// Dialog procedure for the Task Manager dialog
INT_PTR CALLBACK DialogProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
    case WM_INITDIALOG:
        // Initialize global variables
        hListBox = GetDlgItem(hwnd, IDC_LIST1);
        hEditBox = GetDlgItem(hwnd, IDC_EDIT1);
        // Update the process list
        UpdateProcessList();
        return TRUE;

    case WM_COMMAND:
        switch (LOWORD(wParam)) {
        case IDC_BUTTON1:
            // Create Task button clicked
            CreateNewProcess();
            return TRUE;

        case IDC_BUTTON2:
            // Update Task Manager button clicked
            UpdateProcessList();
            return TRUE;

        case IDC_BUTTON3:
            // Terminate Task button clicked
            TerminateSelectedProcess();
            return TRUE;
        }
        case IDC_BUTTON4:
            // Info button clicked
            /*ShowProcessInfo();*/
            return TRUE;
        break;

    case WM_CLOSE:
        EndDialog(hwnd, 0);
        return TRUE;
    }
    return FALSE;
}

// Entry point of the application
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    // Create the main dialog
    DialogBox(hInstance, MAKEINTRESOURCE(IDD_DIALOG1), NULL, DialogProc);

    return 0;
}
