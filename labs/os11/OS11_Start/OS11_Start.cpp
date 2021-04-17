#include <iostream>
#include <Windows.h>
#include <cstdlib>
#include "HashTable.h"
#include <string>

using namespace std;

string key = "my_key";
string value = "my_value";
HT::HTHANDLE* ht = NULL;

void StartHT(LPCSTR FileName) {
	string filePath = FileName;
	char* filePathCost = (char*)filePath.c_str();


	HT::HTHANDLE* ht = HT::Open(filePathCost);
	
	if (ht != NULL) {
		printf("\nHT-Storage Created filename=%s, snapshotinterval=%d, capacity = %d, maxkeylength = %d, maxdatalength = %d",
			filePathCost, ht->SecSnapshotInterval, ht->Capacity, ht->MaxKeyLength, ht->MaxPayloadLength);
		wprintf(L"\nFilemap name: %s", ht->fileMapName);
	}
	else {
		printf("\n some error fail@");
	}

	system("pause");
	if (HT::Snap(ht)) {
		printf("\nsuccessful snap)");
	}else {
		printf("\nfeil snap(");
	}
	HT::Close(ht);
}

int main(int argc, char* argv[])
{
	int capacity = 20, keyLen = 20, valLen = 64, snapTime = 20;
	if (argc == 2) {
	}
	else {
		printf("\nProgram need 1 parameter! argc: %d.\nDefault parameters are used", argc);
		argv[1] = (char*)"map4.txt";
	}
	StartHT(argv[1]);
	return 0;
}