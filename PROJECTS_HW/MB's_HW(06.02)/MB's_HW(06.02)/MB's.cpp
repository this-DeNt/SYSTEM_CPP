#include <windows.h> 
#include <tchar.h> 
#include <iostream>

INT WINAPI _tWinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPTSTR lpszCmdLine, int nCmdShow)
{
	
	TCHAR resume[100] = TEXT("My resume bomba prosto ya vaxui");

	INT resumeLen = _tcsclen(resume);

	for (size_t i = 0; i < 3; ++i) {

		TCHAR buffer[100];
		for (size_t j = 0; j < (3 - i) / resumeLen; ++j) {

			buffer
		}
	}
	return 0;
}	