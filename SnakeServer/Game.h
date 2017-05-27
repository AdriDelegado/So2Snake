#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>
#include <string.h>
#include <tchar.h>

typedef struct {
	DWORD max_number_players;

	char max_number_items;
	DWORD started_snake_size;
	DWORD game_duration;
	DWORD probability_rare_items;
	DWORD number_ai_snakes;
	DWORD list_of_players[20];
}GAME;

//
//void settingDefaultValues(
//	DWORD max_number_players,
//	DWORD max_number_items,
//	DWORD started_snake_size,
//	DWORD game_duration,
//	DWORD probability_rare_items,
//	DWORD number_ai_snakes
//);

void serverInicializaGame(GAME *game);

void mostraSettingFromClient(GAME *game);