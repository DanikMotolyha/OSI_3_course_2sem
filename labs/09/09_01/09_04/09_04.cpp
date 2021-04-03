// 09_04.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
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

#pragma region Task4 //printWatchRowFileTxt
int getFileRowCount(LPWSTR FileName)
{
	int rowCount = 0;
	try
	{
		Sleep(500);
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
		for (int i = 0; i < strlen(buf); i++)
		{
			if (buf[i] == '\n') { rowCount++; }
		}
		CloseHandle(hf);
		return rowCount;
	}
	catch (const char* err)
	{
		cout << "Error: " << err << endl;
		return 0;
	}
}
bool printWatchRowFileTxt(LPWSTR FileName, DWORD mlsec)
{
	char cCurrentPath[FILENAME_MAX];
	if (!_getcwd(cCurrentPath, sizeof(cCurrentPath)))				//GetCurrentDir
	{
		cout << errno << endl;
	}
	cCurrentPath[sizeof(cCurrentPath) - 1] = '\0';					/* not really required */

	printf("The current working directory is %s\n", cCurrentPath);
	//----------------------------------
	wchar_t* wString = new wchar_t[2024];//4096
	MultiByteToWideChar(CP_ACP, 0, cCurrentPath, -1, wString, 4096);
	wchar_t* directory = wString;
	//----------------------------------
	try
	{
		int currentRowCount;
		int prevRowCount;
		HANDLE hf1 = CreateFile(FileName, GENERIC_WRITE | GENERIC_READ, NULL, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
		if (hf1 == INVALID_HANDLE_VALUE)
		{
			throw "create or open file is failed";
		}

		DWORD size1 = GetFileSize(hf1, NULL);
		CloseHandle(hf1);

		currentRowCount = prevRowCount = getFileRowCount(FileName);
		HANDLE dwChangeHandle = HANDLE();
		DWORD dwWaitStatus;

		clock_t t1 = clock();
		clock_t t2 = clock();
		while ((t2 - t1) <= mlsec)
		{
			clock_t t2 = clock();
			Sleep(100);
			dwChangeHandle = FindFirstChangeNotification((LPWSTR)directory, false, FILE_NOTIFY_CHANGE_SIZE);
			if (dwChangeHandle == INVALID_HANDLE_VALUE)
			{
				throw "ChangeNotification is faild";
			}

			dwWaitStatus = WaitForSingleObject(dwChangeHandle, mlsec);
			if (dwWaitStatus == WAIT_OBJECT_0)
			{
				FindCloseChangeNotification(dwChangeHandle);
				if (dwChangeHandle == INVALID_HANDLE_VALUE)
				{
					break;
				}
				LPWSTR fileName = new wchar_t[200]{ L'\0' };

				currentRowCount = getFileRowCount(FileName);
				if (prevRowCount != currentRowCount) {
					cout << "Row count has been changed to " << currentRowCount << " in time " << (float)(t2 - t1) / 1000 << " sec" << endl;

					prevRowCount = currentRowCount;
				}
			}
		}
		return true;
	}
	catch (const char* err)
	{
		cout << "---Error: " << err << endl;
		return false;
	}
}
#pragma endregion

void FourTask() {
	wchar_t file[] = L"file.txt";
	if (printWatchRowFileTxt(file, 120000)) { //2min
		cout << "printWatchRowFileTxt - status code : OK" << endl;
	}
	else {
		cout << "printWatchRowFileTxt - status code : Bad" << endl;
	}
}

int main()
{
	setlocale(LC_ALL, "Rus");
	FourTask();
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
