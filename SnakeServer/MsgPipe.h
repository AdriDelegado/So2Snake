#pragma once
#include "../SnakeOnline/Estruturas.h"
//SERVIDOR

typedef struct {
	GAME *game;
	MAP *map;
	PLAYER *player;
	HANDLE hPipe;
	HANDLE  hThread;
	DWORD dwThreadID;
	LPTSTR lpszPipename;
}MSGPIPESERVIDOR;

void inicialMap(MSGPIPESERVIDOR * msgpipe);

boolean checkPos(DWORD x, DWORD y, MSGPIPESERVIDOR * msgpipe);

void setItem(DWORD item, DWORD x, DWORD y, MSGPIPESERVIDOR * msgpipe);

DWORD getposX(DWORD pos, MSGPIPESERVIDOR * msgpipe);

DWORD getposY(DWORD pos, MSGPIPESERVIDOR * msgpipe);

DWORD getPos(DWORD x, DWORD y, MSGPIPESERVIDOR * msgpipe);
