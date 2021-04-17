#include <iostream>
#include <Windows.h>
#include <cstdlib>
#include "HashTable.h"
#include <string>

using namespace std;


void Do(LPCSTR FileName) {
	string filePath =  FileName;
	char* filePathCost = (char*)filePath.c_str();
	DWORD er;

	HT::HTHANDLE* ht = HT::Open(filePathCost);
	if (ht != NULL) {
		printf("\nHT-Storage Created filename=%s, snapshotinterval=%d, capacity = %d, maxkeylength = %d, maxdatalength = %d",
			filePathCost, ht->SecSnapshotInterval, ht->Capacity, ht->MaxKeyLength, ht->MaxPayloadLength);
	}
	else {
		printf("\n some error fail@: %d", GetLastError());
		return;
	}
	srand(time(NULL));
	HT::Element* element;
	int random;
	int value = 0;
	while (true) {
		random = rand() % 50;
		element = new HT::Element((const void*)&random, 4, &value, 4);
		element = HT::Get(ht, element);
		if (element!=NULL) {
			printf("\nSuccessful getting element with key: %d. Value: %d", *(int*)element->key, *(int*)element->payload);
			value = *(int*)element->payload + 1;
			if (HT::Update(ht, element, &value, 4)) {
				printf("\nSuccessful updating element with key: %d. OldValue: %d", *(int*)element->key, *(int*)element->payload);
			}else{
				printf("\nFail with updating key: %d. Message: %s", random, HT::GetLastHtError(ht));
			}
		}
		else {
			printf("\nFail with getting key: %d. Message: %s", random, HT::GetLastHtError(ht));
		}
		Sleep(1000);
	}


	HT::Close(ht);
}

int main(int argc, char* argv[])
{
	if (argc == 2) {
	}
	else {
		printf("\nProgram need 1 parameter! argc: %d.\nDefault parameters are used", argc);
		argv[1] = (char*)"map4.txt";
	}
	Do(argv[1]);
	return 0;
}