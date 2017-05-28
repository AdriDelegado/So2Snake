#pragma once
#include "Game.h"
#include "Item.h"
#include "Map.h"
#include "Player.h"
#include "Snake.h"

typedef struct {
	GAME *game;
	ITEM *item;
	MAP *map;
	PLAYER *player;
	SNAKE *snake;
}MSGPIPE;