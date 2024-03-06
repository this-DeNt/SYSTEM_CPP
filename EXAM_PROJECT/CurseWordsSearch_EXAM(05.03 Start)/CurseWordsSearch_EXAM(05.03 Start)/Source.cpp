#include <windows.h>
#include <string>
#include <set>
#include <fstream>
#include <vector>
#include <algorithm>
#include <Commctrl.h>
#include "resource.h"

std::set<std::wstring> words;
std::vector<std::wstring> foundWords;
int curseWords = 0;
HANDLE mtx;
bool censorshipDone = false;

HWND hProgressBar;

void searchFiles() {
    WIN32_FIND_DATAW findFileData;
    HANDLE hFind = FindFirstFileW(L"./*.txt", &findFileData);
    if (hFind != INVALID_HANDLE_VALUE) {
        do {
            std::wstring filePath = findFileData.cFileName;
            std::wifstream file(filePath);
            std::wstring word;
            while (file >> word) {
                WaitForSingleObject(mtx, INFINITE);
                if (words.find(word) != words.end()) {
                    foundWords.push_back(word);
                    curseWords++;
                }
                ReleaseMutex(mtx);
            }
        } while (FindNextFileW(hFind, &findFileData) != 0);
        FindClose(hFind);
    }
}

void writeStatistics() {
    if (!censorshipDone)
        return;

    std::wofstream file("Statistics.txt");
    for (const auto& word : foundWords) {
        file << word << std::endl;
    }
    file << "Total curse words: " << curseWords << std::endl;
}

void censorFiles() {
    WIN32_FIND_DATAW findFileData;
    HANDLE hFind = FindFirstFileW(L"./*.txt", &findFileData);
    if (hFind != INVALID_HANDLE_VALUE) {
        do {
            std::wstring filePath = findFileData.cFileName;
            std::wifstream file(filePath);
            std::wstring content((std::istreambuf_iterator<wchar_t>(file)), std::istreambuf_iterator<wchar_t>());
            file.close();
            for (const auto& word : words) {
                size_t pos = content.find(word);
                while (pos != std::wstring::npos) {
                    content.replace(pos, word.length(), L"*******");
                    pos = content.find(word, pos + 7);
                }
            }
            std::wofstream outFile(filePath + L"_censored.txt");
            outFile << content;
            outFile.close();
        } while (FindNextFileW(hFind, &findFileData) != 0);
        FindClose(hFind);
    }
    censorshipDone = true;
}

void UpdateProgressBar(HWND hProgressBar, int pos) {
    SendMessage(hProgressBar, PBM_SETPOS, pos, 0);
}

void StartSearch(HWND hwnd) {
    EnableWindow(GetDlgItem(hwnd, IDC_BUTTON_START), FALSE);

    SendMessage(hProgressBar, PBM_SETRANGE, 0, MAKELPARAM(0, 100));
    SendMessage(hProgressBar, PBM_SETSTEP, 1, 0);

    for (int i = 0; i <= 100; i++) {
        UpdateProgressBar(hProgressBar, i);
        Sleep(100);
    }

    MessageBox(hwnd, L"Search Complete", L"Info", MB_OK);

    HANDLE t1 = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)searchFiles, NULL, 0, NULL);
    WaitForSingleObject(t1, INFINITE);
    CloseHandle(t1);

    HANDLE t2 = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)writeStatistics, NULL, 0, NULL);
    WaitForSingleObject(t2, INFINITE);
    CloseHandle(t2);

    HANDLE t3 = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)censorFiles, NULL, 0, NULL);
    WaitForSingleObject(t3, INFINITE);
    CloseHandle(t3);

    EnableWindow(GetDlgItem(hwnd, IDC_BUTTON_START), TRUE);
}

INT_PTR CALLBACK DlgProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_INITDIALOG:
        mtx = CreateMutex(NULL, FALSE, NULL);
        hProgressBar = GetDlgItem(hwnd, IDC_PROGRESS_CURSE_SEARCH);
        return TRUE;
    case WM_COMMAND:
        switch (LOWORD(wParam))
        {
        case IDC_BUTTON_MAN_IMP:
        {
            TCHAR buffer[256];
            GetDlgItemText(hwnd, IDC_EDIT_MAN_IMP, buffer, 256);
            std::wstring word(buffer);
            WaitForSingleObject(mtx, INFINITE);
            words.insert(word);
            ReleaseMutex(mtx);
            SendDlgItemMessage(hwnd, IDC_LIST_CURSE, LB_ADDSTRING, 0, (LPARAM)buffer);
            MessageBox(hwnd, L"Added successfully", L"Success", MB_OK);
            return TRUE;
        }
        case IDC_BUTTON_DEL_CURSE:
        {
            int idx = SendDlgItemMessage(hwnd, IDC_LIST_CURSE, LB_GETCURSEL, 0, 0);
            if (idx != LB_ERR)
            {
                TCHAR buffer[256];
                SendDlgItemMessage(hwnd, IDC_LIST_CURSE, LB_GETTEXT, idx, (LPARAM)buffer);
                std::wstring word(buffer);
                WaitForSingleObject(mtx, INFINITE);
                words.erase(word);
                ReleaseMutex(mtx);
                SendDlgItemMessage(hwnd, IDC_LIST_CURSE, LB_DELETESTRING, idx, 0);
            }
            return TRUE;
        }
        case IDC_BUTTON_FILE_IMP:
            DialogBox(NULL, MAKEINTRESOURCE(IDD_DIALOG_FILE_PATH), hwnd, DlgProc);
            return TRUE;
        case IDC_BUTTON_PATH_ENTER:
        {
            TCHAR buffer[256];
            GetDlgItemText(hwnd, IDC_EDIT_PATH, buffer, 256);
            std::wifstream file(buffer);
            if (file)
            {
                std::wstring word;
                while (file >> word)
                {
                    words.insert(word);
                    SendDlgItemMessage(hwnd, IDC_LIST_CURSE, LB_ADDSTRING, 0, (LPARAM)word.c_str());
                }
                MessageBox(hwnd, L"Import successful", L"Success", MB_ICONINFORMATION | MB_OK);
            }
            else
            {
                MessageBox(hwnd, L"Import failed", L"Error", MB_ICONERROR | MB_OK);
            }
            EndDialog(hwnd, 0);
            return TRUE;
        }
        case IDC_BUTTON_START:
            StartSearch(hwnd);
            return TRUE;
        }
        break;
    case WM_CLOSE:
        CloseHandle(mtx);
        EndDialog(hwnd, 0);
        return TRUE;
    }
    return FALSE;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    DialogBox(hInstance, MAKEINTRESOURCE(IDD_DIALOG_CURSE_MAIN), NULL, DlgProc);
    return 0;
}
