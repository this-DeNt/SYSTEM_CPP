// pch.cpp: файл исходного кода, соответствующий предварительно скомпилированному заголовочному файлу

#include "pch.h"

// При использовании предварительно скомпилированных заголовочных файлов необходим следующий файл исходного кода для выполнения сборки.

std::set<std::wstring> words;
std::vector<std::wstring> foundWords;
int curseWords = 0;
HANDLE mtx;
bool censorshipDone = false;

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
                    MessageBox(NULL, L"Found Curse Word", L"Info", MB_OK);
                    Sleep(3000); // delay
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
    MessageBox(NULL, L"Statistics gathered", L"Info", MB_OK);
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
    MessageBox(NULL, L"Censored all the files", L"Info", MB_OK);
    writeStatistics(); // Write statistics after all files are censored
}