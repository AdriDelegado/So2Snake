#pragma once
//CLIENTE
#include "Estruturas.h"


typedef struct {
	GAME *game;
	MAP *map;
	PLAYER *player;
	HANDLE hPipe;
	HANDLE  hThread;
	DWORD dwThreadID;
	LPTSTR lpszPipename;
}MSGPIPECLIENTE;

