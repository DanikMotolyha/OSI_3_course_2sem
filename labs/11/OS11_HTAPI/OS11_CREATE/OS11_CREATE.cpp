#include <iostream>
//#pragma comment(lib, "d:\\Study\\лабараторные\\03_02\\ОСИ\\2021\\labs\\11\\OS11_HTAPI\\Debug\\OS11HTAPI.lib")
#include "OS11_HTAPI.h";

using namespace std;


HT::HTHANDLE* ht = NULL;

void CreateHT(LPCSTR FileName, int capacity, int keyLen, int valLen, int snapTime) {
	string filePath = FileName;
	char* filePathCost = (char*)filePath.c_str();


	HT::HTHANDLE* ht = HT::Create(capacity, snapTime, keyLen, valLen, filePathCost);

	if (ht != NULL) {
		printf("\nHT-Storage Created filename=%s, snapshotinterval=%d, capacity = %d, maxkeylength = %d, maxdatalength = %d",
			filePathCost, snapTime, capacity, keyLen, valLen);
	}
	else {
		printf("\n------\nERROR\n------");
	}

	HT::Close(ht);
}

int main(int argc, char* argv[])
{
	int capacity = 20, keyLen = 20, valLen = 64, snapTime = 200;
	if (argc == 6) {
		int temp;

		temp = atoi(argv[2]);
		if (temp != 0)
			capacity = temp;

		temp = atoi(argv[3]);
		if (temp != 0)
			keyLen = temp;

		temp = atoi(argv[4]);
		if (temp != 0)
			valLen = temp;

		temp = atoi(argv[5]);
		if (temp != 0)
			snapTime = temp;

	}
	else {
		printf("\nProgram need 5 parameters! argc: %d.\nDefault parameters are used", argc);
		argv[1] = (char*)"txt.txt";
	}
	CreateHT(argv[1], capacity, keyLen, valLen, snapTime);
	return 0;
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
