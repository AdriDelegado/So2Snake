#include "ClientMenu.h"


void mainMenu() {


	
	_tprintf(TEXT("BEM VINDO\n"));
	_tprintf(TEXT("\t\tSingle Player\t-1-\n"));
	_tprintf(TEXT("\t\tMultiPlayer\t-2-\n"));
}

void menuSettingUpGame() {
	BOOL fSucess = FALSE;
	DWORD cbToWrite, cbWritten;
	GAME * auxGame = (GAME *)malloc(sizeof(GAME));
	PLAYER * auxPlayer = (PLAYER *)malloc(sizeof(PLAYER));
	_tprintf(TEXT("Single Player\n"));
	_tprintf(TEXT("Setting Up The Game\n"));

	_tprintf(TEXT("\t\tPlayer Name:\n"));
	_tscanf(TEXT("%s"), &auxPlayer->player_name);
	_tprintf(TEXT("\t\tMaximum Number of Items on the Fild\n"));
	_tscanf(TEXT("%s"), &auxGame->max_number_items);

	_tprintf(TEXT("\t\tStarting Size of the Snake\n"));
	_tscanf(TEXT("%s"), &auxGame->started_snake_size);
	_tprintf(TEXT("\t\Game Duration\n"));
	_tscanf(TEXT("%s"), &auxGame->game_duration);
	_tprintf(TEXT("\t\tProbability of rare Items to Spawn\n"));
	_tscanf(TEXT("%s"), &auxGame->);
	_tprintf(TEXT("\t\tNumber of Enemy Snakes\n"));
	//scan
	_tprintf(TEXT("\t\tNumber of Horizontal Tilles\n"));
	//scan
	_tprintf(TEXT("\t\tNumber of Vertical Tilles\n"));
	//scan
}


void menuMultiplayer1() {

	_tprintf(TEXT("Multiplayer\n"));
	_tprintf(TEXT("\t\tHost a Game\t-1-"));
	_tprintf(TEXT("\t\tJoin a Game\t-2-"));
}

void menuMultiplayer2() {

}