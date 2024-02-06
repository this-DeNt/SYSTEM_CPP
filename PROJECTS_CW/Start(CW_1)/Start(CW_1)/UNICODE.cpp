#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <tchar.h>

using namespace std;

int main(int argc, char* argv[]) {

	// Task 1
	_TCHAR exampleStr[30] = _TEXT("Hello World");

	wcout << "Initial String: " << exampleStr << endl;

	for (int i = 0; i < sizeof(exampleStr); ++i) {

		if (exampleStr[i] == 32) {

			exampleStr[i] = 9;
		}
	}

	wcout << "Output String: " << exampleStr << endl;

	// Task 2
	size_t chars = 0, nums = 0, others = 0;
	for (int i = 0; i < sizeof(exampleStr); ++i) {

		if (exampleStr[i] >= 101 && exampleStr[i] <= 132 || exampleStr[i] >= 141 && exampleStr[i] <= 172) {

			++chars;
		}
		else if(exampleStr[i] >= 60 && exampleStr[i] <= 71) {

			++nums;
		}
		else {

			++others;
		}
	}

	wcout << chars << endl << nums << endl << others;
}