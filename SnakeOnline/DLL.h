// DLL.h - Contains declaration of Function class  
#pragma once  
#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>
#include <string.h>
#include <tchar.h>
#include <memory.h> 
#include "Estruturas.h"
#include "..\SnakeServer\MsgPipe.h"

#define SHMEMSIZE 4096
#define GAMEBUFSIZE sizeof(GAME)

#ifdef SNAKE_API  
#define SNAKE_API __declspec(dllexport)   
#else  
#define SNAKE_API __declspec(dllimport)   
#endif  


extern "C"
{

	TCHAR szNAME[] = TEXT("fmMsgSpace");
	//Variável global da DLL
	extern SNAKE_API int nDLL;

	//Funções a serem exportadas/importadas
	SNAKE_API BOOL CommandVerifier(TCHAR comando[]);
	SNAKE_API BOOL SharedMemmoryInicializer();
	SNAKE_API BOOL SharedMemmoryCloser();
	SNAKE_API BOOL PlayerAdder(TCHAR nome[]);
	SNAKE_API int PlayerList();
	SNAKE_API void GameConfigurer(MSGPIPESERVIDOR  msgpipe);

}