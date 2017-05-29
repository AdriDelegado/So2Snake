#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>
#include <string.h>
#include <tchar.h>

typedef struct {
	char playerCreator_name[20];
	DWORD number_players;

	DWORD max_number_items;
	DWORD started_snake_size;
	DWORD game_duration;
	DWORD probability_rare_items;
	DWORD number_ai_snakes;
	DWORD horizontal_sizeX;
	DWORD vertical_sizeY;
}GAME;

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
	char player_name[20];
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



void inicialMap(DWORD height, DWORD width);

boolean checkPos(DWORD pos);

boolean setItem(DWORD pos);

int convertCoordenatesIntoPos(int linhas, int colunas);