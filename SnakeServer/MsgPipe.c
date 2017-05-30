#include "MsgPipe.h"
#pragma once

void inicialMap(MSGPIPESERVIDOR * msgpipe, MAP * map)
{
	map->height = msgpipe->map->height;
	map->width = msgpipe->map->width;
	
	for (int i = 0; i <= map->width; i++) {
		for (int j = 0; j <= map->height; j++) {
			msgpipe->map->tiles[i][j] = 0;
			
		}
	}

	map->tiles = msgpipe->map->tiles;

}

boolean checkPos(int x, int y, MAP * map)
{
	if(map->tiles[x][y] != 0)
		return TRUE;
	return FALSE;
}

void setItem(int item, int x, int y, MAP * map)
{
	map->tiles[x][y] = item;
}

int getposX(int pos, MAP * map)
{
	for (int i = 0; i < map->height; i++) {
		for (int j = 0; j < map->width; j++) {
			if (pos == map->tiles[i][j]) {
				return i;
			}
		}
	}
	return -1;
}

int getposY(int pos, MAP * map)
{
	for (int i = 0; i < map->height; i++) {
		for (int j = 0; j < map->width; j++) {
			if (pos == map->tiles[i][j]) {
				return j;
			}
		}
	}
	return -1;
}

int getPos(int x, int y, MAP * map)
{
	for (int i = 0; i < map->height; i++) {
		for (int j = 0; j < map->width; j++) {
			if (i == x && j == y) {
				return i*j;
			}
		}
	}
	return -1;
}

