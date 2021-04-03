// 09_01.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//
/*
	types of files
	FILE_TYPE_CHAR    0x0002
	FILE_TYPE_DISK    0x0001
	FILE_TYPE_PIPE    0x0003
	FILE_TYPE_REMOTE  0x8000
	FILE_TYPE_UNKNOWN 0x0000
*/
#include <iostream>
#include <direct.h>
#include <Windows.h>
using namespace std;
#include <locale.h>
#pragma region Task1 //printFileInfo, printFileTxt
BOOL printFileInfo(LPWSTR FileName)
{
	try
	{
		HANDLE hf = CreateFile(FileName, GENERIC_WRITE | GENERIC_READ, NULL, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
		if (hf == INVALID_HANDLE_VALUE)
		{
			throw "create or open file is failed";
		}
		cout << "Create or open file is successful" << endl;
		char* fn = (char*)malloc(100);
		wcstombs(fn, FileName, 100);
		LPBY_HANDLE_FILE_INFORMATION file = new _BY_HANDLE_FILE_INFORMATION();
		SYSTEMTIME stCreate, stUpdate, stCreateLP, stUpdateLP;

		if (!GetFileInformationByHandle(hf, file))
		{
			throw "GetInformationByHandle is faild";
		}

		if (!FileTimeToSystemTime(&file->ftCreationTime, &stCreate))
		{
			throw "FileTimeToSystemTime is faild";
		}

		if (!FileTimeToSystemTime(&file->ftLastWriteTime, &stUpdate))
		{
			throw "FileTimeToSystemTime is faild";
		}

		SystemTimeToTzSpecificLocalTime(NULL, &stCreate, &stCreateLP);
		SystemTimeToTzSpecificLocalTime(NULL, &stUpdate, &stUpdateLP);

		cout << "\nFile name: " << fn << endl;
		cout << "File type: " << GetFileType(hf) << endl;
		cout << "File size: " << GetFileSize(hf, NULL) << " byte" << endl;
		cout << "File creation time: "
			<< stCreateLP.wDay
			<< "." << stCreateLP.wMonth
			<< "." << stCreateLP.wYear
			<< " " << stCreateLP.wHour
			<< ":" << stCreateLP.wMinute
			<< ":" << stCreateLP.wSecond << endl;
		cout << "Last update time: "
			<< stUpdateLP.wDay
			<< "." << stUpdateLP.wMonth
			<< "." << stUpdateLP.wYear
			<< " " << stUpdateLP.wHour
			<< ":" << stUpdateLP.wMinute
			<< ":" << stUpdateLP.wSecond << endl;

		CloseHandle(hf);
		return true;
	}
	catch (const char* err)
	{
		cout << "Error: " << err << endl;
		return false;
	}
}
BOOL printFileTxt(LPWSTR FileName)
{
	try
	{
		HANDLE hf = CreateFile(FileName, GENERIC_READ, NULL, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
		if (hf == INVALID_HANDLE_VALUE)
		{
			throw "create or open file is failed";
		}
		cout << "Create or open file is successful" << endl;

		DWORD n = 1024;
		char buf[1024];
		ZeroMemory(buf, sizeof(buf));
		if (!ReadFile(hf, &buf, 1024, &n, NULL))
		{
			throw "ReadFile is failed";
		}
		cout << "\n-------------------------\n" << buf << "\n-------------------------\n" << endl;

		CloseHandle(hf);
		return true;
	}
	catch (const char* err)
	{
		cout << "Error: " << err << endl;
		return false;
	}
}
#pragma endregion



void FirstTask() {
	wchar_t file[] = L"file.txt";
	if (printFileInfo(file)) {
		cout << endl << "get info - status code: OK" << endl;
	}
	else {
		cout << endl << "get info - status code: Bad" << endl;
	}
	cout << "--------------------------" << endl;
	if (printFileTxt(file)) {
		cout << endl << "read File - status code: OK" << endl;
	}
	else {
		cout << endl << "read File - status code: Bad" << endl;
	}
}




int main()
{
	setlocale(LC_ALL, "Rus");
	FirstTask();
	system("pause");
}

// Запуск программы: CTRL+F5 или меню "Отладка" > "Запуск без отладки"
// Отладка программы: F5 или меню "Отладка" > "Запустить отладку"

// Советы по началу работы 
//   1. В окне обозревателя решений можно добавлять файлы и управлять ими.
//   2. В окне Team Explorer можно подключиться к системе управления версиями.
//   3. В окне "Выходные данные" можно просматривать выходные данные сборки и другие сообщения.
//   4. В окне "Список ошибок" можно просматривать ошибки.
//   5. Последовательно выберите пункты меню "Проект" > "Добавить новый элемент", чтобы создать файлы кода, или "Проект" > "Добавить существующий элемент", чтобы добавить в проект существующие файлы кода.
//   6. Чтобы снова открыть этот проект позже, выберите пункты меню "Файл" > "Открыть" > "Проект" и выберите SLN-файл.
