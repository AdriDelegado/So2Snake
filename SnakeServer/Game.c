#include "Game.h"

//void settingDefaultValues(GAME *game,DWORD max_number_players,
//	DWORD max_number_items,
//	DWORD started_snake_size,
//	DWORD game_duration,
//	DWORD probability_rare_items,
//	DWORD number_ai_snakes
//	) {
//	game = (GAME*)malloc(sizeof(GAME));
//	game->max_number_players = max_number_players;
//	game->max_number_items = max_number_items;
//	game->started_snake_size = started_snake_size;
//	game->game_duration = game_duration;
//	game->probability_rare_items = probability_rare_items;
//	game->number_ai_snakes = number_ai_snakes;
//	int i;
//
//	for (i = 0;i < 20;i++) {
//		game->list_of_players[i] = 0;
//	}

//}

void serverInicializaGame(GAME *game) {
	
	
			game = (GAME*)malloc(sizeof(GAME));
			game->max_number_players = 20;
			game->max_number_items = 10;
			game->started_snake_size = 2;
			game->game_duration = 5;
			game->probability_rare_items = 10;
			game->number_ai_snakes = 0;
			int i;
		
			for (i = 0;i < 20;i++) {
				game->list_of_players[i] = 0;
			}
}


void mostraSettingFromClient(GAME *game) {

}