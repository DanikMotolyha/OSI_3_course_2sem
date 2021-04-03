#pragma once
#include <Windows.h>

namespace HT    // HT API
{
	// API HT - программный интерфейс для доступа к НТ-хранилищу 
	//          НТ-хранилище предназначено для хранения данных в ОП в формате ключ/значение
	//          Персистестеность (сохранность) данных обеспечивается с помощью snapshot-менханизма 
	//          Create - создать  и открыть HT-хранилище для использования   
	//          Open   - открыть HT-хранилище для использования
	//          Insert - создать элемент данных
	//          Delete - удалить элемент данных    
	//          Get    - читать  элемент данных
	//          Update - изменить элемент данных
	//          Snap   - выпонить snapshot
	//          Close  - выполнить Snap и закрыть HT-хранилище для использования
	//          GetLastError - получить сообщение о последней ошибке


	struct HTHANDLE    // блок управления HT
	{
		HTHANDLE();
		HTHANDLE(int Capacity, int SecSnapshotInterval, int MaxKeyLength, int MaxPayloadLength, const char FileName[512]);
		int     Capacity;											// емкость хранилища в количестве элементов 
		int     SecSnapshotInterval;								// переодичность сохранения в сек. 
		int     MaxKeyLength;										// максимальная длина ключа
		int     MaxPayloadLength;									// максимальная длина данных
		int		CurrentSize = 0;									// текущая длина, количество элемов
		char    FileName[512];										// имя файла 
		HANDLE  File;												// File HANDLE != 0, если файл открыт
		HANDLE  FileMapping;										// Mapping File HANDLE != 0, если mapping создан  
		LPVOID  Addr;												// Addr != NULL, если mapview выполнен  
		char    LastErrorMessage[512] = "0";						// сообщение об последней ошибке или 0x00  
		time_t  lastsnaptime;										// дата последнего snap'a (time())  

		HANDLE  timerThread;
		wchar_t mutexName[512];
		HANDLE event_killme = CreateEvent(NULL, TRUE, FALSE, L"timerThread");
	};

	struct Element   // элемент 
	{
		Element();
		Element(const void* key, int keylength);                                             // for Get
		Element(const void* key, int keylength, const void* payload, int  payloadlength);    // for Insert
		Element(Element* oldelement, const void* newpayload, int  newpayloadlength);         // for update
		const void* key;					// значение ключа 
		int keylength;						// рахмер ключа
		const void* payload;				// данные 
		int payloadlength;					// размер данных
	};

	HTHANDLE* Create   //  создать HT             
	(
		int	  Capacity = 1000,								// емкость хранилища
		int   SecSnapshotInterval = 20,					    // переодичность сохранения в сек.
		int   MaxKeyLength = 10,							// максимальный размер ключа
		int   MaxPayloadLength = 10,						// максимальный размер данных
		const char  FileName[512] = "default.ht"			// имя файла 
	); 	// != NULL успешное завершение  

	HTHANDLE* Open     //  открыть HT             
	(
		const char    FileName[512]         // имя файла 
	); 	// != NULL успешное завершение  

	BOOL Snap         // выполнить Snapshot
	(
		HTHANDLE* hthandle           // управление HT (File, FileMapping)
	);


	BOOL Close        // Snap и закрыть HT  и  очистить HTHANDLE
	(
		HTHANDLE* hthandle           // управление HT (File, FileMapping)
	);	//  == TRUE успешное завершение   


	BOOL Insert      // добавить элемент в хранилище
	(
		HTHANDLE* hthandle,            // управление HT
		Element* element              // элемент
	);	//  == TRUE успешное завершение 


	BOOL Delete      // удалить элемент в хранилище
	(
		HTHANDLE* hthandle,            // управление HT (ключ)
		Element* element              // элемент 
	);	//  == TRUE успешное завершение 

	Element* Get     //  читать элемент в хранилище
	(
		HTHANDLE* hthandle,            // управление HT
		Element* element              // элемент 
	); 	//  != NULL успешное завершение 


	BOOL Update     //  именить элемент в хранилище
	(
		HTHANDLE* hthandle,            // управление HT
		Element* oldelement,          // старый элемент (ключ, размер ключа)
		const void* newpayload,          // новые данные  
		int             newpayloadlength     // размер новых данных
	); 	//  != NULL успешное завершение 

	char* GetLastError  // получить сообщение о последней ошибке
	(
		HTHANDLE* ht                         // управление HT
	);

	void print                               // распечатать элемент 
	(
		const Element* element              // элемент 
	);
	void printAll                               // распечатать все элементы
	(
		HTHANDLE* ht
	);

};
