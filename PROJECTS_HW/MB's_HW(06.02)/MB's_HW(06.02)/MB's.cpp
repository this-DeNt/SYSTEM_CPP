#include <windows.h> 
#include <tchar.h> 

INT WINAPI _tWinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPTSTR lpszCmdLine, int nCmdShow)
{
	char resume[100] = "Eto pizda naxui\0";
	for (char* i = resume; i != "\0"; ++i) {

		MessageBox( 0, TEXT("Реализация алгоритма программы непосредственно в функции WinMain"), TEXT("Окно сообщения"), MB_OK | MB_ICONINFORMATION);
	}
	return 0;
}	