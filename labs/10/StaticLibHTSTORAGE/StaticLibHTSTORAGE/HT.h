#pragma once
#include <Windows.h>

namespace HT    // HT API
{
	// API HT - ����������� ��������� ��� ������� � ��-��������� 
	//          ��-��������� ������������� ��� �������� ������ � �� � ������� ����/��������
	//          ���������������� (�����������) ������ �������������� � ������� snapshot-���������� 
	//          Create - �������  � ������� HT-��������� ��� �������������   
	//          Open   - ������� HT-��������� ��� �������������
	//          Insert - ������� ������� ������
	//          Delete - ������� ������� ������    
	//          Get    - ������  ������� ������
	//          Update - �������� ������� ������
	//          Snap   - �������� snapshot
	//          Close  - ��������� Snap � ������� HT-��������� ��� �������������
	//          GetLastError - �������� ��������� � ��������� ������


	struct HTHANDLE    // ���� ���������� HT
	{
		HTHANDLE();
		HTHANDLE(int Capacity, int SecSnapshotInterval, int MaxKeyLength, int MaxPayloadLength, const char FileName[512]);
		int     Capacity;											// ������� ��������� � ���������� ��������� 
		int     SecSnapshotInterval;								// ������������� ���������� � ���. 
		int     MaxKeyLength;										// ������������ ����� �����
		int     MaxPayloadLength;									// ������������ ����� ������
		int		CurrentSize = 0;									// ������� �����, ���������� ������
		char    FileName[512];										// ��� ����� 
		HANDLE  File;												// File HANDLE != 0, ���� ���� ������
		HANDLE  FileMapping;										// Mapping File HANDLE != 0, ���� mapping ������  
		LPVOID  Addr;												// Addr != NULL, ���� mapview ��������  
		char    LastErrorMessage[512] = "0";						// ��������� �� ��������� ������ ��� 0x00  
		time_t  lastsnaptime;										// ���� ���������� snap'a (time())  

		HANDLE  timerThread;
		wchar_t mutexName[512];
		HANDLE event_killme = CreateEvent(NULL, TRUE, FALSE, L"timerThread");
	};

	struct Element   // ������� 
	{
		Element();
		Element(const void* key, int keylength);                                             // for Get
		Element(const void* key, int keylength, const void* payload, int  payloadlength);    // for Insert
		Element(Element* oldelement, const void* newpayload, int  newpayloadlength);         // for update
		const void* key;					// �������� ����� 
		int keylength;						// ������ �����
		const void* payload;				// ������ 
		int payloadlength;					// ������ ������
	};

	HTHANDLE* Create   //  ������� HT             
	(
		int	  Capacity = 1000,								// ������� ���������
		int   SecSnapshotInterval = 20,					    // ������������� ���������� � ���.
		int   MaxKeyLength = 10,							// ������������ ������ �����
		int   MaxPayloadLength = 10,						// ������������ ������ ������
		const char  FileName[512] = "default.ht"			// ��� ����� 
	); 	// != NULL �������� ����������  

	HTHANDLE* Open     //  ������� HT             
	(
		const char    FileName[512]         // ��� ����� 
	); 	// != NULL �������� ����������  

	BOOL Snap         // ��������� Snapshot
	(
		HTHANDLE* hthandle           // ���������� HT (File, FileMapping)
	);


	BOOL Close        // Snap � ������� HT  �  �������� HTHANDLE
	(
		HTHANDLE* hthandle           // ���������� HT (File, FileMapping)
	);	//  == TRUE �������� ����������   


	BOOL Insert      // �������� ������� � ���������
	(
		HTHANDLE* hthandle,            // ���������� HT
		Element* element              // �������
	);	//  == TRUE �������� ���������� 


	BOOL Delete      // ������� ������� � ���������
	(
		HTHANDLE* hthandle,            // ���������� HT (����)
		Element* element              // ������� 
	);	//  == TRUE �������� ���������� 

	Element* Get     //  ������ ������� � ���������
	(
		HTHANDLE* hthandle,            // ���������� HT
		Element* element              // ������� 
	); 	//  != NULL �������� ���������� 


	BOOL Update     //  ������� ������� � ���������
	(
		HTHANDLE* hthandle,            // ���������� HT
		Element* oldelement,          // ������ ������� (����, ������ �����)
		const void* newpayload,          // ����� ������  
		int             newpayloadlength     // ������ ����� ������
	); 	//  != NULL �������� ���������� 

	char* GetLastError  // �������� ��������� � ��������� ������
	(
		HTHANDLE* ht                         // ���������� HT
	);

	void print                               // ����������� ������� 
	(
		const Element* element              // ������� 
	);
	void printAll                               // ����������� ��� ��������
	(
		HTHANDLE* ht
	);

};
