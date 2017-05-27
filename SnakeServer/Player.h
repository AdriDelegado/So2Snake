#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>
#include <string.h>
#include <tchar.h>

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