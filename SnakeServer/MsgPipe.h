#pragma once
#include "Estruturas.h"


typedef struct {
	GAME *game;

	MAP *map;
	PLAYER *player;

}MSGPIPE;

void inicialMap(MSGPIPE * msgpipe, MAP * map);

boolean checkPos(int x, int y, MAP * map);

void setItem(int item, int x, int y, MAP * map);

int getposX(int pos, MAP * map);

int getposY(int pos, MAP * map);

int getPos(int x, int y, MAP * map);
