#include <iostream>
#include <Windows.h>
#include <cstdlib>
#include "HashTable.h"
#include <string>

using namespace std;

string key = "my_key";
string value = "my_value";
HT::HTHANDLE* ht = NULL;

void CreateHT(LPCSTR FileName, int capacity, int keyLen, int valLen, int snapTime) {
	string filePath =  FileName;
	char* filePathCost = (char*)filePath.c_str();


	HT::HTHANDLE* ht = HT::Create(capacity, snapTime, keyLen, valLen, filePathCost);

	if (ht != NULL) {
		printf("\nHT-Storage Created filename=%s, snapshotinterval=%d, capacity = %d, maxkeylength = %d, maxdatalength = %d",
			filePathCost,snapTime,capacity,keyLen,valLen);
	}
	else {
		printf("\n some error fail@");
	}

	HT::Close(ht);
}

int main(int argc, char* argv[])
{
	int capacity = 20, keyLen = 20, valLen = 64, snapTime = 20;
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
		argv[1] = (char*)"map4.txt";
	}
	CreateHT(argv[1], capacity,keyLen,valLen,snapTime);
	return 0;
}