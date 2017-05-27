// DLL.cpp : Defines the exported functions for the DLL application.  
// Compile by using: cl /EHsc /DMATHLIBRARY_EXPORTS /LD MathLibrary.cpp  

  
#include "DLL.h"  

unsigned createSharedMemory(ContrDataServer cdata) {

	// POR ESTA PARTE NA DLL;

	cdata.hMapFile = CreateFileMapping(
		INVALID_HANDLE_VALUE,
		NULL,
		PAGE_READWRITE,
		0,
		GAMEBUFSIZE,
		szNAME);

	if (cdata.hMapFile == NULL) {
		_tprintf(TEXT(
			"Houve azar na memoria partilhada ( erro %d).\n"), GetLastError());
		return 1;
	}
	_tprintf(TEXT("Vou Criar a View da memoria Partilhada.\n"));

	cdata.gameBuf = (GAME*)MapViewOfFile(
		cdata.hMapFile,
		FILE_MAP_ALL_ACCESS,
		0,
		0,
		GAMEBUFSIZE);
	if (cdata.gameBuf == NULL) {
		_tprintf(TEXT(
			"A view da memoria partilhada nao foi criada ( erro: %d). \n"), GetLastError());
		CloseHandle(cdata.hMapFile);
		return 1;
	}
}
	