// 09_02.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include <direct.h>
#include <Windows.h>
using namespace std;
#include <locale.h>

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

#pragma region Task2 //delRowFileTxt
BOOL delRowFileTxt(LPWSTR FileName, DWORD row) {
	HANDLE hf;
	HANDLE hf1;
	try
	{
		hf = CreateFile(FileName, GENERIC_WRITE | GENERIC_READ, NULL, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
		if (hf == INVALID_HANDLE_VALUE)
		{
			throw "create or open file is failed";
		}
		int rowCount = 1;
		DWORD n = NULL;
		char buf[1024];
		char newBuf[1024];
		int lenNewBuf = 0;
		ZeroMemory(buf, sizeof(buf));
		ZeroMemory(newBuf, sizeof(newBuf));
		if (!ReadFile(hf, &buf, 1024, &n, NULL))
		{
			throw "ReadFile is failed";
		}

		CloseHandle(hf);

		for (int i = 0; i < strlen(buf); i++)
		{
			if (rowCount != row)
			{
				newBuf[lenNewBuf] = buf[i]; lenNewBuf++;
			}
			if (buf[i] == '\n')
			{
				rowCount++;
			}
		}
		hf1 = CreateFile(FileName, GENERIC_WRITE | GENERIC_READ, NULL, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
		if (hf1 == INVALID_HANDLE_VALUE)
		{
			throw "Create or open file is faild";
		}

		if (!WriteFile(hf1, newBuf, strlen(newBuf), &n, NULL))
		{
			throw "WriteFile is faild";
		}
		CloseHandle(hf1);
		return true;
	}
	catch (const char* err)
	{
		cout << "---Error: " << err << endl;
		CloseHandle(hf);
		return false;
	}
}
#pragma endregion

void SecondTask() {
	int rows[4] = { 1, 3, 8 , 10 };
	wchar_t file[] = L"file.txt";
	for (int i = 0; i < 4; i++) {
		if (delRowFileTxt(file, rows[i])) {
			cout << "Delete is successful\n" << endl;
			Sleep(1000);
		}
		else {
			cout << "Delete is faild" << endl;
		}
	}
	if (printFileTxt(file)) {
		cout << "read File - status code: OK" << endl;
	}
	else {
		cout << endl << "read File - status code: Bad" << endl;
	}
}

int main()
{
	setlocale(LC_ALL, "Rus");
	SecondTask();
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
