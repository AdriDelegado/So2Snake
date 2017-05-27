#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>
#include <string.h>
#include <tchar.h>

typedef struct {
	DWORD max_number_players[20];

	char max_number_items;
	DWORD started_snake_size;
	DWORD game_duration;
	DWORD probability_rare_items;
	DWORD number_ai_snakes;
}GAME;