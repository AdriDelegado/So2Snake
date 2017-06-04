#include "MsgPipe.h"

void inicialMap(MSGPIPESERVIDOR * msgpipe)
{
	for (int i = 0; i <= msgpipe->map->width; i++) {
		for (int j = 0; j <= msgpipe->map->height; j++) {
			msgpipe->map->tiles[i][j] = 0;
		}
	}
}

boolean checkPos(DWORD x, DWORD y, MSGPIPESERVIDOR * msgpipe)
{
	if(msgpipe->map->tiles[x][y] != 0)
		return TRUE;
	return FALSE;
}

void setItem(DWORD item, DWORD x, DWORD y, MSGPIPESERVIDOR * msgpipe)
{
	msgpipe->map->tiles[x][y] = item;
}

DWORD getposX(DWORD pos, MSGPIPESERVIDOR * msgpipe)
{
	for (int i = 0; i < msgpipe->map->height; i++) {
		for (int j = 0; j < msgpipe->map->width; j++) {
			if (pos == msgpipe->map->tiles[i][j]) {
				return i;
			}
		}
	}
	return -1;
}

DWORD getposY(DWORD pos, MSGPIPESERVIDOR * msgpipe)
{
	for (int i = 0; i < msgpipe->map->height; i++) {
		for (int j = 0; j < msgpipe->map->width; j++) {
			if (pos == msgpipe->map->tiles[i][j]) {
				return j;
			}
		}
	}
	return -1;
}

DWORD getPos(DWORD x, DWORD y, MSGPIPESERVIDOR * msgpipe)
{
	for (int i = 0; i < msgpipe->map->height; i++) {
		for (int j = 0; j < msgpipe->map->width; j++) {
			if (i == x && j == y) {
				return i*j;
			}
		}
	}
	return -1;
}

