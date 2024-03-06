#pragma once

#ifdef EXAM_EXPORTS
#define EXAM_API __declspec(dllexport)
#else
#define EXAM_API __declspec(dllimport)
#endif

#define WIN32_LEAN_AND_MEAN             // Исключите редко используемые компоненты из заголовков Windows
// Файлы заголовков Windows
#include <windows.h>
#include <string>
#include <fstream>
#include <algorithm>
#include <vector>
#include <set>

extern "C" EXAM_API void searchFiles();
extern "C" EXAM_API void writeStatistics();
extern "C" EXAM_API void censorFiles();