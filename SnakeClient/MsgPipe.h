#pragma once
//CLIENTE
#include "../SnakeOnline/Estruturas.h"


typedef struct {
	GAME *game;
	MAP *map;
	PLAYER *player;
	HANDLE hPipe;
	HANDLE  hThread;
	DWORD dwThreadID;
	LPTSTR lpszPipename;
}MSGPIPECLIENTE;

