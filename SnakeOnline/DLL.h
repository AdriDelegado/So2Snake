// DLL.h - Contains declaration of Function class  
#pragma once  
#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>
#include <string.h>
#include <tchar.h>
#include <memory.h> 
#include "..\SnakeServer\Game.h"

#define SHMEMSIZE 4096
#define GAMEBUFSIZE sizeof(GAME)
typedef struct _ControlDataServer {
	HANDLE hMapFile;
	GAME * gameBuf;
	int ThreadDeveContinuar;
}ContrDataServer;
#ifdef SNAKE_API  
#define SNAKE_API __declspec(dllexport)   
#else  
#define SNAKE_API __declspec(dllimport)   
#endif  


extern "C"
{

	TCHAR szNAME[] = TEXT("fmMsgSpace");
	SNAKE_API unsigned createSharedMemory(ContrDataServer cdata);
}