// OS11_HTAPI.cpp : Определяет экспортируемые функции для DLL.
//

#include "pch.h"
#include "framework.h"
#include "OS11_HTAPI.h"
#include "pch.h"
#include <string>
#include <iostream>
#include <algorithm>
#pragma warning(disable : 4996)
using namespace std;

namespace HT
{
	HTHANDLE::HTHANDLE() {
	}
	HTHANDLE::HTHANDLE(int Capacity, int SecSnapshotInterval, int MaxKeyLength, int MaxPayloadLength, const char FileName[512]) {
		this->Capacity = Capacity;
		this->SecSnapshotInterval = SecSnapshotInterval;
		this->MaxKeyLength = MaxKeyLength;
		this->MaxPayloadLength = MaxPayloadLength;
		ZeroMemory(this->FileName, strlen(this->FileName));
		strcpy_s(this->FileName, strlen(FileName) + 1, FileName);
	}
	Element::Element() {
	}
	Element::Element(const void* key, int keylength) { // get
		this->keylength = keylength;
		this->key = (void*)key;
	}

	Element::Element(const void* key, int keylength, const void* payload, int  payloadlength) {  // insert
		this->keylength = keylength;
		this->key = (void*)key;
		this->payloadlength = payloadlength;
		this->payload = (void*)payload;
	}

	Element::Element(Element* oldelement, const void* newpayload, int  newpayloadlength) { // update
		oldelement->payload = newpayload;
		oldelement->payloadlength = newpayloadlength;
	}

	static wchar_t* charToWChar(const char* text)
	{
		const size_t size = strlen(text) + 1;
		wchar_t* wText = new wchar_t[size];
		mbstowcs(wText, text, size);
		return wText;
	}
	const wchar_t* GetWC(const char* c)
	{
		const size_t cSize = strlen(c) + 1;
		wchar_t* wc = new wchar_t[cSize];
		mbstowcs(wc, c, cSize);

		return wc;
	}

	bool CheckMemotyForAccess(HTHANDLE* hthandle, bool dontNeedBossOpened = false) {

		LPVOID buf = malloc(sizeof(HTHANDLE));
		if (!ReadProcessMemory(
			GetCurrentProcess(),
			hthandle,
			buf,
			sizeof(HTHANDLE),
			NULL
		)) {
			free(buf);
			return false;
		}
		free(buf);
		return true;
	}


	void checkElement(const HTHANDLE* hthandle, const Element* element) {
		if (element->keylength > hthandle->MaxKeyLength) {
			throw "key length was incorrect (more than acceptable)";
		}
		if (element->payloadlength > hthandle->MaxPayloadLength) {
			throw "PayLoad length was incorrect (more than acceptable)";
		}
		if (strlen((const char*)element->payload) != element->payloadlength) {
			throw "PayLoad length was incorrect";
		}
		if (strlen((const char*)element->key) != element->keylength) {
			throw "key length was incorrect";
		}
	}

	BYTE* FindElement(HTHANDLE* ht, void* key, int keyLength) {
		BYTE* pointer = NULL;
		Element* elem;
		for (int counter = 0; counter < ht->CurrentSize; counter++) {
			pointer = (BYTE*)(ht)+sizeof(HTHANDLE) + counter * (sizeof(Element) + ht->MaxKeyLength + ht->MaxPayloadLength); // run for elements
			elem = (Element*)pointer;
			elem->key = (pointer + sizeof(Element));
			elem->payload = (pointer + sizeof(Element) + ht->MaxKeyLength);
			//compare string key with key element
			if (memcmp(elem->key, key, keyLength) == 0) {
				return pointer;
			}
		}
		return NULL;
	}


	DWORD WINAPI SnapShotTimer(LPVOID hthandle) {
		HTHANDLE* ht = (HTHANDLE*)hthandle;
		int interval = 10;
		if (!ReadProcessMemory(
			GetCurrentProcess(),
			hthandle,
			NULL,
			sizeof(HTHANDLE),
			NULL
		)) {
			interval = ht->SecSnapshotInterval;
		}
		while (WaitForSingleObject(ht->event_killme, 0) != WAIT_OBJECT_0) {
			if (!ReadProcessMemory(
				GetCurrentProcess(),
				hthandle,
				NULL,
				sizeof(HTHANDLE),
				NULL
			)) {
				Snap(ht);
			}
			printf("\ni sleep\n");;
			Sleep(1000 * interval);
		}
		return 0;
	}
	BOOL startAsyncSnap(HTHANDLE* hthandle) {
		hthandle->timerThread = CreateThread(
			NULL,                   // default security attributes
			0,                      // use default stack size  
			SnapShotTimer,			// thread function name
			hthandle,				// argument to thread function 
			0,                      // use default creation flags 
			NULL);					// returns the thread identifier 

		if (hthandle->timerThread == NULL) {
			throw "creating async thread snap";
		}
	}



	BOOL Snap         // выполнить Snapshot
	(
		HTHANDLE* hthandle           // управление HT (File, FileMapping)
	) {
		HANDLE mutex = CreateMutex(NULL, FALSE, hthandle->mutexName);
		WaitForSingleObject(mutex, INFINITE);
		//printf("\n\x1B[32msnapshot command is started\033[0m\n");
		printf("\nsnapshot command is started\n");
		if (ReadProcessMemory(
			GetCurrentProcess(),
			hthandle,
			NULL,
			sizeof(HTHANDLE),
			NULL
		)) {
			return false;
		}
		cout << "\nSNAP\nSNAP\nSNAP\n";
		//writing memory into created snap file 
		int totalHtSize = sizeof(HTHANDLE);
		totalHtSize += hthandle->Capacity * (sizeof(Element) + hthandle->MaxKeyLength + hthandle->MaxPayloadLength);
		if (!WriteFile(
			hthandle->File,
			hthandle,
			totalHtSize,
			NULL,
			NULL
		)) {
			strcpy_s((*hthandle).LastErrorMessage, strlen("error writing file") + 1, "error writing file");
		}
		time_t now = time(NULL);
		(*hthandle).lastsnaptime = now;
		std::tm* ptm = std::localtime(&hthandle->lastsnaptime);
		char time[32];
		std::strftime(time, 32, "%a, %d.%m.%Y %H:%M:%S", ptm);
		//printf("\n\x1B[32mcommand snapshot completed\n\tTIME: %s\033[0m\n", time);
		printf("\ncommand snapshot completed\n\tTIME: %s \n", time);
		ReleaseMutex(mutex);
		return true;
	}

	Element* GetFromMemory(HTHANDLE* hthandle, BYTE* pointer) {
		Element* elem = (Element*)pointer;
		elem->key = (pointer + sizeof(Element));
		elem->payload = (pointer + sizeof(Element) + hthandle->MaxKeyLength);
		return elem;
	}

#pragma region CRUD

	BOOL Insert      // добавить элемент в хранилище
	(
		HTHANDLE* hthandle,            // управление HT
		Element* element              // элемент
	) {	//  == TRUE успешное завершение
		try {
			HANDLE mutex = CreateMutex(NULL, FALSE, hthandle->mutexName);
			WaitForSingleObject(mutex, INFINITE);
			if (FindElement((HTHANDLE*)hthandle, (void*)element->key, element->keylength) != NULL) {
				throw "element with this key already exist";
			}
			if (hthandle->CurrentSize >= hthandle->Capacity) {
				throw "ht storage is full";
			}
			checkElement(hthandle, element);
			//take pointer for next element
			BYTE* pointer = (BYTE*)(hthandle)+sizeof(HTHANDLE) + hthandle->CurrentSize * (sizeof(Element) + hthandle->MaxKeyLength + hthandle->MaxPayloadLength);
			Element* newElement = (Element*)pointer;

			newElement->key = (pointer + sizeof(Element));
			newElement->payload = (pointer + sizeof(Element) + hthandle->MaxKeyLength);
			newElement->keylength = element->keylength;
			newElement->payloadlength = element->payloadlength;

			CopyMemory((void*)newElement->key, element->key, newElement->keylength);
			CopyMemory((void*)newElement->payload, element->payload, newElement->payloadlength);

			(*hthandle).CurrentSize += 1;
			ReleaseMutex(mutex);
			return true;
		}
		catch (const char* error) {
			cout << endl << "error was made: " << error << endl;
			strcpy_s((*hthandle).LastErrorMessage, strlen(error) + 1, error);
		}
		return false;
	}

	Element* Get						//  читать элемент в хранилище
	(
		HTHANDLE* hthandle,            // управление HT
		Element* element               // элемент 
	) {
		HANDLE mutex = NULL;
		try {
			mutex = CreateMutex(NULL, FALSE, hthandle->mutexName);
			WaitForSingleObject(mutex, INFINITE);
			if (element->keylength > hthandle->MaxKeyLength) {
				throw "key length was incorrect (more than acceptable)";
			}
			BYTE* pointer = FindElement((HTHANDLE*)hthandle, (void*)element->key, element->keylength);
			if (pointer == NULL) {
				ReleaseMutex(mutex);
				return NULL;
			}
			else {
				Element* elem = GetFromMemory(hthandle, pointer);
				ReleaseMutex(mutex);
				return elem;
			}
		}
		catch (const char* error) {
			cout << endl << "error was made: " << error << endl;
			strcpy_s((*hthandle).LastErrorMessage, strlen(error) + 1, error);
		}
		ReleaseMutex(mutex);
		return NULL;
	}
	BOOL Update     //  именить элемент в хранилище
	(
		HTHANDLE* hthandle,            // управление HT
		Element* oldelement,           // старый элемент (ключ, размер ключа)
		const void* newpayload,				 // новые данные  
		int	newpayloadlength				 // размер новых данных
	) { 	//  != NULL успешное завершение 
		HANDLE mutex = NULL;
		try {
			mutex = CreateMutex(NULL, FALSE, hthandle->mutexName);
			WaitForSingleObject(mutex, INFINITE);
			if (newpayloadlength > hthandle->MaxPayloadLength) {
				throw "PayLoad length was incorrect (more than acceptable)";
			}
			if (strlen((const char*)newpayload) != newpayloadlength) {
				throw "PayLoad length was incorrect";
			}
			BYTE* pointer = FindElement((HTHANDLE*)hthandle, (void*)oldelement->key, oldelement->keylength);
			if (pointer != NULL) {
				Element* elem = GetFromMemory((HTHANDLE*)hthandle, pointer);
				elem->payloadlength = newpayloadlength;
				CopyMemory((void*)elem->payload, newpayload, newpayloadlength);
				ReleaseMutex(mutex);
				return true;
			}
			else {
				ReleaseMutex(mutex);
				return false;
			}
		}
		catch (const char* error) {
			cout << endl << "error was made: " << error << endl;
			strcpy_s((*hthandle).LastErrorMessage, strlen(error) + 1, error);
		}
		ReleaseMutex(mutex);
		return false;
	}

	BOOL Delete      // удалить элемент в хранилище
	(
		HTHANDLE* hthandle,            // управление HT (ключ)
		Element* element              // элемент 
	) {	//  == TRUE успешное завершение 
		HANDLE mutex = NULL;
		try {
			mutex = CreateMutex(NULL, FALSE, hthandle->mutexName);
			WaitForSingleObject(mutex, INFINITE);
			/*			0									   1							2
				/----------------/					   /----------------/			 /----------------/
				/-----elem 1-----/					   /-----elem 1-----/			 /-----elem 1-----/
				/-----elem 2-----/		 pointer	   								 /-----elem 3-----/
				/-----elem 3-----/		 AfterPointer  /-----elem 3-----/			 /-----elem 4-----/
				/-----elem 4-----/					   /-----elem 4-----/			 /-----elem 5-----/
				/-----elem 5-----/					   /-----elem 5-----/			 /----------------/
				/----------------/					   /----------------/

				for del elem 2
				1 - zeroMemory fro elem 2
				2 - CopyMemory to pointer from afterPointer
			*/
			BYTE* pointer = NULL;
			Element* elem;
			int counter = 0;
			for (; counter < hthandle->CurrentSize; counter++) {
				pointer = (BYTE*)(hthandle)+sizeof(HTHANDLE) + counter * (sizeof(Element) + hthandle->MaxKeyLength + hthandle->MaxPayloadLength); // run for elements
				elem = (Element*)pointer;
				elem->key = (pointer + sizeof(Element));
				elem->payload = (pointer + sizeof(Element) + hthandle->MaxKeyLength);
				//compare string key with key element
				if (memcmp(elem->key, element->key, element->keylength) == 0) {
					break;
				}
			}
			if (pointer != NULL) {
				ZeroMemory(pointer, (sizeof(Element) + hthandle->MaxKeyLength + hthandle->MaxPayloadLength));
				BYTE* afterPointer = pointer + (sizeof(Element) + hthandle->MaxKeyLength + hthandle->MaxPayloadLength);
				int memory = (hthandle->CurrentSize - counter) * (hthandle->MaxKeyLength + hthandle->MaxPayloadLength + sizeof(Element));
				CopyMemory(pointer, afterPointer, memory);
				hthandle->CurrentSize -= 1;
				ReleaseMutex(mutex);
				return true;
			}
			else {
				ReleaseMutex(mutex);
				return false;
			}
		}
		catch (const char* error) {
			cout << endl << "error was made: " << error << endl;
			strcpy_s((*hthandle).LastErrorMessage, strlen(error) + 1, error);
		}
		ReleaseMutex(mutex);
		return false;
	}

#pragma endregion

#pragma region NAVIGATE

	HTHANDLE* Create   //  создать HT             
	(
		int	  Capacity,								 // емкость хранилища
		int   SecSnapshotInterval,					 // переодичность сохранения в сек.
		int   MaxKeyLength,							 // максимальный размер ключа
		int   MaxPayloadLength,						 // максимальный размер данных
		const char  FileName[512]					 // имя файла 
	) { // != NULL успешное завершение  

		// need to create space for new ht
		HANDLE handleFile = NULL;
		HANDLE handleFileMapping = NULL;
		wchar_t mutexName[512];
		wchar_t* file = charToWChar(FileName);
		try {
			const wchar_t* fileName = GetWC(FileName);
			wcsncpy(mutexName, GetWC(FileName), strlen(FileName) + 1);
			delete fileName;
			handleFile = CreateFile(
				file,
				GENERIC_READ | GENERIC_WRITE,
				NULL,
				NULL,
				OPEN_ALWAYS,
				FILE_ATTRIBUTE_NORMAL,
				NULL
			);
			if (handleFile == INVALID_HANDLE_VALUE)
				throw "error opening or creating file";

			/*
				need to malloc space for store (this structure about file)
				1 - header - HTHANDLE					sizeof(HTHANDLE)
				2 - space for data (key:data)			Capacity * (sizeof(Element) + MaxKeyLength + MaxPayloadLength)
				|--------------------|
				|-------header-------|		1064byte = 1060 summary size params + 2 constructors (2 links)
				|--------------------|		sizeof(int) *4 + sizeof(char[512]) *2 + sizeof(HANDLE) * 2 + sizeof(LPVOID) + sizeof(time_t)
				|--------------------|		+++ 2 constructors (2 links)
				|--------------------|
				|--------data--------|
				|------key:data------|
				|--------------------|
			*/
			//step 1 create HTHANDLE and write to file (create header)

			HTHANDLE* header = new HTHANDLE(Capacity, SecSnapshotInterval, MaxKeyLength, MaxPayloadLength, FileName);
			ZeroMemory(header->mutexName, wcslen(header->mutexName));
			wcsncpy(header->mutexName, mutexName, wcslen(mutexName) + 1);
			DWORD written;
			cout << "\ninit new storage header\n\tsize: " << sizeof(*header) << "\n\tfile: " << FileName << endl;
			WriteFile(
				handleFile,
				header,
				sizeof(*header),
				&written,
				NULL
			);

			//step 2 create data storage and write to file

			int dataSize = Capacity * (MaxKeyLength + MaxPayloadLength + sizeof(Element));
			void* data = malloc(dataSize);
			cout << "init new storage data\n\tsize: " << dataSize << "\n\tfile: " << FileName << endl;
			WriteFile(
				handleFile,
				data,
				dataSize,
				&written,
				NULL
			);

			//clear space

			delete header;
			free(data);

			//claer data space (delete old data ~ trash)
			handleFileMapping = CreateFileMapping(
				handleFile,
				NULL,
				PAGE_READWRITE,
				0, 0, file);
			if (!handleFileMapping) throw "error create filemapping";

			LPVOID lp = MapViewOfFile(
				handleFileMapping,
				FILE_MAP_ALL_ACCESS,
				0, 0, 0);
			if (!lp) throw "error create mapping file";

			//clean old bytes in data store*/
			ZeroMemory((BYTE*)lp + sizeof(HTHANDLE), Capacity * (sizeof(Element) + MaxKeyLength + MaxPayloadLength));
			CloseHandle(handleFile);
			CloseHandle(handleFileMapping);
			UnmapViewOfFile(lp);
			delete(file);
			return Open(FileName);
		}
		catch (const char* error) {
			delete(file);
			cout << endl << "error was made: " << error << endl;
			CloseHandle(handleFile);
			CloseHandle(handleFileMapping);
		}
		return NULL;
	}

	HTHANDLE* Open     //  открыть HT             
	(
		const char    FileName[512]         // имя файла 
	) { 	// != NULL успешное завершение  
		HANDLE handleFile = NULL;
		HANDLE handleFileMapping = NULL;
		wchar_t* file = charToWChar(FileName);
		try {

			handleFile = CreateFile(
				file,
				GENERIC_READ | GENERIC_WRITE,
				NULL,
				NULL,
				OPEN_ALWAYS,
				FILE_ATTRIBUTE_NORMAL,
				NULL
			);
			if (handleFile == INVALID_HANDLE_VALUE)
				throw "error opening or creating file";

			//create storage in RAM 
			handleFileMapping = CreateFileMapping(
				handleFile,
				NULL,
				PAGE_READWRITE,
				0, 0, file);
			if (!handleFileMapping) throw "error create filemapping";

			LPVOID lp = MapViewOfFile(
				handleFileMapping,
				FILE_MAP_ALL_ACCESS,
				0, 0, 0);
			if (!lp) throw "error create mapping file";
			// try to write this info to file
			HTHANDLE* htHandle = (HTHANDLE*)(BYTE*)lp;
			cout << "\nOpen HT storage form file"
				<< "\n\tname file: " << (*htHandle).FileName
				<< "\n\tcapacity: " << htHandle->Capacity
				<< "\n\tmax Key Length: " << htHandle->MaxKeyLength
				<< "\n\tmax Payload Length: " << htHandle->MaxPayloadLength
				<< "\n\tspanshot interval: " << htHandle->SecSnapshotInterval
				<< endl;
			htHandle->File = handleFile;
			htHandle->FileMapping = handleFileMapping;
			htHandle->Addr = (BYTE*)lp + sizeof(HTHANDLE);



			if (!startAsyncSnap(htHandle))
				throw "cannot start asyncSnap";
			delete(file);
			return htHandle;
		}
		catch (const char* error) {
			delete(file);
			cout << endl << "error was made: " << error << endl;
		}

	}

	BOOL Close        // Snap и закрыть HT  и  очистить HTHANDLE
	(
		HTHANDLE* hthandle           // управление HT (File, FileMapping)
	) {	//  == TRUE успешное завершение  
		Snap(hthandle);
		SetEvent(hthandle->event_killme);
		WaitForSingleObject(hthandle->timerThread, INFINITE);
		if (hthandle->timerThread != NULL)
			CloseHandle(hthandle->timerThread);
		if (hthandle->File != NULL)
			CloseHandle(hthandle->File);
		if (hthandle->FileMapping != NULL)
			CloseHandle(hthandle->FileMapping);
		if (!UnmapViewOfFile(hthandle)) throw "error was made: unmapping file error";
	}

#pragma endregion

	void printAll(HTHANDLE* ht) {					// распечатать все элементы
		BYTE* pointer = NULL;
		Element* elem;
		cout << "all data from ht storage (file: " << (*ht).FileName << " )\n";
		cout << "----------------------------------------------------\n";
		for (int counter = 0; counter < ht->CurrentSize; counter++) {
			pointer = (BYTE*)(ht)+sizeof(HTHANDLE) + counter * (sizeof(Element) + ht->MaxKeyLength + ht->MaxPayloadLength); // run for elements
			elem = (Element*)pointer;
			elem->key = (pointer + sizeof(Element));
			elem->payload = (pointer + sizeof(Element) + ht->MaxKeyLength);
			print(elem);
		}
		cout << "----------------------------------------------------\n";
	}


	void print(const Element* element) {			// распечатать элемент
		cout << "key: " << (char*)element->key << " data: " << (char*)element->payload << endl;
	}

}

