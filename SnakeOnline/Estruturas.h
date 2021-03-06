#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>
#include <string.h>
#include <tchar.h>



typedef struct {
	DWORD item_id;

	char item_name[20];
	//coordinates where the item is placed 
	DWORD coordinate_x;
	DWORD coordinate_y;
}ITEM;


typedef struct {
	DWORD height;
	DWORD width;
	DWORD **tiles;
}MAP;

typedef struct {
	DWORD player_id;
	DWORD size;
	TCHAR player_name[20];
	DWORD score;
	//keys for directions on the keyboard
	DWORD up_key;
	DWORD down_key;
	DWORD left_key;
	DWORD rigth_key;
	// variable that says if the player can move the snake
	boolean in_game;
}PLAYER;

typedef struct {
	DWORD snake_id;
	char direction;
	DWORD player_id;
	DWORD size;
	DWORD speed;
	DWORD coordinate_x;
	DWORD coordinate_y;
}SNAKE;


typedef struct {
	TCHAR playerCreator_name[20];
	PLAYER playerArray[20];
	SNAKE snakeArray[20];
	ITEM itemsArray[20];
	DWORD max_number_players;
	DWORD actual_players;
	BOOLEAN gameExist;
	DWORD max_number_items;
	DWORD started_snake_size;
	DWORD game_duration;
	FLOAT probability_rare_items;
	DWORD number_ai_snakes;
	DWORD horizontal_sizeX;
	DWORD vertical_sizeY;
}GAME;
