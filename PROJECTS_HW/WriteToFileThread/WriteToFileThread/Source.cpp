#include <windows.h>
#include <string>
#include <iostream>
#include <fstream>
#include <thread>
#include <mutex>
#include "resource.h"
#include <random>
#include <ctime>

// Function to generate random wstring
std::wstring generateRandomString(size_t length) {
    static const wchar_t characters[] =
        L"0123456789"
        L"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        L"abcdefghijklmnopqrstuvwxyz";

    std::wstring randomString;
    randomString.reserve(length);

    std::mt19937 rng(static_cast<unsigned int>(std::time(nullptr)));
    std::uniform_int_distribution<size_t> distribution(0, sizeof(characters) / sizeof(characters[0]) - 2);

    for (size_t i = 0; i < length; ++i) {
        randomString.push_back(characters[distribution(rng)]);
    }

    return randomString;
}

// Mutex for thread safety
std::mutex mtx;

// Function to write random wstring to file
void writeToFile() {
    while (true) {
        mtx.lock(); // Lock mutex
        std::wstring randomString = generateRandomString(100); // Generate random wstring
        std::wofstream file("music.txt", std::ios_base::app); // Open file for appending
        if (file.is_open()) {
            file << randomString << std::endl; // Write wstring to file
            file.close(); // Close file
        }
        else {
            // Handle error if file cannot be opened
        }
        mtx.unlock(); // Unlock mutex
        std::this_thread::sleep_for(std::chrono::seconds(1)); // Sleep for 1 second
    }
}

// Dialog procedure
INT_PTR CALLBACK DialogProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
    case WM_INITDIALOG:
        // Start thread to write to file
        std::thread(writeToFile).detach();
        return TRUE;
    case WM_COMMAND:
        switch (LOWORD(wParam)) {
        case IDOK:
            EndDialog(hwnd, 0); // Close the dialog on button click
            return TRUE;
        }
        break;
    case WM_CLOSE:
        EndDialog(hwnd, 0); // Close the dialog when X button is clicked
        return TRUE;
    }
    return FALSE;
}

// Entry point
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    DialogBox(hInstance, MAKEINTRESOURCE(IDD_DIALOG1), NULL, DialogProc); // Display dialog
    return 0;
}
